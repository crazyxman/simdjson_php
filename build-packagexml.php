<?php

/**
 * Generate package.xml for pecl extensions.
 * Works for new and existing extensions.
 *
 * Add it to `Makefile.frag`
 *   package.xml: php_$(PHP_PECL_EXTENSION).h
 *   	$(PHP_EXECUTABLE) build-packagexml.php
 *
 * Usage
 *   cat ~/my-release-notes.txt | make package.xml
 *
 * @author Arnold Daniels <arnold@jasny.net>
 * @license PHP License
 * @license MIT
 */

declare (strict_types = 1);

set_exception_handler(function (Throwable $exception): void {
    $msg = ($exception instanceof RuntimeException ? "" : "Unexpected error: ") . $exception->getMessage();

    fwrite(STDERR, "\033[0;31m" /* red */ . $msg . "\033[0m" . "\n");
    exit(1);
});

if (!extension_loaded('dom') || !extension_loaded('simplexml') || !extension_loaded('spl')) {
    throw new RuntimeException("Following extensions are required: DOM, SimpleXML and SPL");
}

// 1. Load package.xml and create release
$package = (function (): PackageXMLElement {
    return file_exists('package.xml')
        ? simplexml_load_file('package.xml', PackageXMLElement::class)
        : PackageXMLElement::create();
})();

$release = new Release();

// 2. Process php_{extname}.h
(function () use ($package, $release): void {
    $extname = (string)$package->name ?: getenv('PHP_PECL_EXTENSION') ?: null;

    if ($extname !== null) {
        $filename = "php_{$extname}.h";

        if (!file_exists($filename)) {
            throw new RuntimeException("{$filename} not found");
        }
    } else {
        $filename = glob('php_*.h')[0] ?? null;

        if ($filename === null) {
            throw new RuntimeException("Couldn't find the main header file (php_*.h)");
        }

        $extname = preg_replace('/^php_(.+)\.h$/', '$1', $filename);
    }

    if ((string)$package->name === '') {
        $package->name = $extname;
    }


    $macroPrefix = strtoupper(pathinfo($filename, PATHINFO_FILENAME));
    $contents = file_get_contents($filename);

    preg_match_all("/^[ \t]*#define\s+{$macroPrefix}_(?<key>\w+)[ \t]+\"(?<value>.+)\"/m", $contents, $matches,
        PREG_PATTERN_ORDER);
    $macros = array_combine($matches['key'], $matches['value']);

    // Package name
    if (isset($macros['EXTNAME'])) {
        if ((string)$package->name !== '' && (string)$package->name !== $macros['EXTNAME']) {
            throw new RuntimeException("Package name '{$package->name}' (package.xml) doesn't match "
                . "{$macroPrefix}_EXTNAME '{$macros['EXTNAME']}' ($filename)");
        }
    }

    // Release version
    if (!isset($macros['VERSION'])) {
        throw new RuntimeException("$filename does not contain {$macroPrefix}_VERSION macro");
    }
    $release->version = $macros['VERSION'];

    if (strpos($release->version, 'dev') !== false) {
        throw new RuntimeException("Development versions shouldn't be released ({$macros['VERSION']}). "
            . "Please change {$macroPrefix}_VERSION in $filename.");
    }
    if ($release->existsIn($package->changelog)) {
        throw new RuntimeException("Version {$macros['VERSION']} already released. "
            . "Please change {$macroPrefix}_VERSION in $filename.");
    }
})();

// 3. Copy info from package to release
(function () use ($package, $release): void {
    $release->license = (string)$package->license;
    $release->licenseUri = (string)$package->license['uri'];

    if (strpos($release->version, 'alpha') !== false) {
        $release->stability = 'alpha';
    } else if (strpos($release->version, 'beta') !== false || strpos($release->version, 'RC') !== false) {
        $release->stability = 'beta';
    } else if (substr($release->version, 0, 1) === '0') {
        $release->stability = ((string)$package->stability->release) ?: 'alpha';
    } else {
        $release->stability = 'stable';
    }

    if ($release->isMajor((string)$package->version->release)) {
        $release->apiVersion = $release->version;
        $release->apiStability = $release->stability;
    } else {
        $release->apiVersion = (string)$package->version->api;
        $release->apiStability = (string)$package->stability->api;
    }
})();

// 4. Get release notes from stdin
$release->notes = (function (string $name, string $version): string {
    if (function_exists('posix_isatty') && posix_isatty(STDIN)) {
        fwrite(STDOUT, "Enter the release notes for $name $version (end with Ctrl-D):\n");
    }

    $notes = '';

    do {
        $notes .= fread(STDIN, 1024);
    } while (!feof(STDIN));

    return trim($notes);
})((string)$package->name, $release->version);

// 5. Add release to package
if (!isset($package->changelog)) {
    $package->addChild('changelog');
}
$release->update($package->changelog->prependChild('release'));
$release->update($package);

// 6. Removes files that no longer exist from package.xml
(function () use ($package): void {
    $fileElements = $package->xpath('p:contents//p:file');

    foreach ($fileElements as $element) {
        $path = join("/", array_map('strval', $element->xpath('ancestor-or-self::*[@name!="/"]/@name')));
        
        if (!file_exists($path)) {
            unset($element[0]);
        }
    };
})();

// 7. Add new files to package.xml
(function () use ($package): void {
    $ext = ['c', 'h', 'phpt'];

    $dir = new RecursiveDirectoryIterator('.', FilesystemIterator::SKIP_DOTS | FilesystemIterator::CURRENT_AS_PATHNAME);
    $itDir = new RecursiveIteratorIterator($dir);
    $itReg = new RegexIterator($itDir, '~^./(.+\.(?:' . join('|', $ext) . '))$~', RegexIterator::GET_MATCH);
    $files = iterable_column($itReg, 1);

    $newFiles = new CallbackFilterIterator($files, function ($path) use ($package): bool {
        $file = basename($path);
        $dirs = dirname($path) !== '.' ? explode('/', dirname($path)) : [];
        array_unshift($dirs, '/');

        $xpath = 'p:dir[@name="' . join('"]/p:dir[@name="', $dirs) . '"]/p:file[@name="' . $file . '"]';
        return count($package->contents->xpath($xpath)) === 0;
    });

    if (is_dir('.git')) {
        $newFiles = gitignore_filter($newFiles);
    }

    foreach ($newFiles as $file) {
        $dirs = dirname($file) !== '.' ? explode('/', dirname($file)) : [];

        $dirElement = array_reduce($dirs, function (SimpleXMLElement $parent, string $dir): SimpleXMLElement {
            $cur = $parent->xpath('p:dir[@name="' . $dir . '"]')[0] ?? null;

            if ($cur === null) {
                $cur = $parent->addChild('dir');
                $cur['name'] = $dir;
            }

            return $cur;
        }, $package->contents->dir[0]);

        $fileElement = $dirElement->addChild('file');
        $fileElement['name'] = basename($file);
        $fileElement['role'] = pathinfo($file, PATHINFO_EXTENSION) === 'phpt' ? 'test' : 'src';
    }
})();

// 8. Remove empty dirs from package.xml
(function () use ($package): void {
    $emptyDirs = $package->xpath('p:contents//p:dir[not(descendant::*[local-name()="file"])]');

    foreach (array_reverse($emptyDirs) as $element) { // reverse so children are deleted first
        unset($element[0]);
    }
})();

// 9. Sort files
(function () use ($package): void {
    $dirElements = $package->xpath('p:contents//p:dir');

    $sorter = function(SimpleXMLElement $first, SimpleXMLElement $second): int {
        return
            (($first->getName() !== 'file') <=> ($second->getName() !== 'file')) ?:
            strcasecmp((string)$first['name'], (string)$second['name']);
    };

    foreach ($dirElements as $element) {
        $element->sort($sorter);
    }
})();

// 10. Save package.xml
$package->asXML('package.xml');

// :) friendly message
echo "\033[0;32m", /* green */ "Updated package.xml for {$package->name} {$release->version}", "\033[0;0m", "\n";

// ================= classes and functions ====================

class Release
{
    public $version;
    public $apiVersion;
    public $stability;
    public $apiStability;

    public $license;
    public $licenseUri;
    public $notes;

    public function isMajor(string $oldVersion): bool
    {
        if ($oldVersion === '') {
            return true;
        }

        [$major, $minor] = explode('.', $this->version);
        [$oldMajor, $oldMinor] = explode('.', $oldVersion);

        return ($major !== $oldMajor) || ($major === '0' && $minor !== $oldMinor);
    }

    public function existsIn(SimpleXMLElement $changelog): bool
    {
        if (!isset($changelog->release)) {
            return false;
        }

        foreach ($changelog->release as $release) {
            if ((string)($release->version->release) === $this->version) {
                return true;
            }
        }

        return false;
    }

    public function update(SimpleXMLElement $element)
    {
        $noTime = isset($element->date) && !isset($element->time);

        $element->date = date('Y-m-d');
        if (!$noTime) {
            $element->time = date('H:i:s');
        }
        
        $element->version->release = $this->version;
        $element->version->api = $this->apiVersion;
        $element->stability->release = $this->stability;
        $element->stability->api = $this->apiStability;

        $element->license = $this->license;
        $element->license['uri'] = $this->licenseUri;

        $indent = str_repeat(' ', count($element->xpath('ancestor::*')) + 1);
        $element->notes = "\n" . $this->notes . "\n$indent";
    }
};

class PackageXMLElement extends SimpleXMLElement
{
    private const BLANK_PACKAGE = <<<XML
<?xml version="1.0" encoding="UTF-8"?>
<package version="2.0" xmlns="http://pear.php.net/dtd/package-2.0" xmlns:tasks="http://pear.php.net/dtd/tasks-1.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://pear.php.net/dtd/tasks-1.0 http://pear.php.net/dtd/tasks-1.0.xsd http://pear.php.net/dtd/package-2.0 http://pear.php.net/dtd/package-2.0.xsd">
 <name></name>
 <channel>pecl.php.net</channel>
 <summary></summary>
 <description></description>
 <lead>
  <name></name>
  <user></user>
  <email></email>
  <active>yes</active>
 </lead>
 <date></date>
 <version>
  <release></release>
  <api></api>
 </version>
 <stability>
  <release></release>
  <api></api>
 </stability>
 <license uri="http://www.php.net/license/3_0.txt">PHP License</license>
 <notes></notes>
 <contents>
  <dir name="/">
   <file name="config.m4" role="src"/>
   <file name="config.m32" role="src"/>
   <file name="CREDITS" role="doc"/>
   <file name="LICENSE" role="doc"/>
   <file name="README.md" role="doc"/>
  </dir>
 </contents>
 <dependencies>
  <required>
   <php>
    <min>7.0.0</min>
   </php>
   <pearinstaller>
    <min>1.4.3</min>
   </pearinstaller>
  </required>
 </dependencies>
 <providesextension></providesextension>
 <extsrcrelease />
 <changelog></changelog>
</package>
XML;

    public static function create(): self
    {
        return new static(self::BLANK_PACKAGE);
    }

    public function prependChild($name, $value = '')
    {
        $dom = dom_import_simplexml($this);

        $new = $dom->insertBefore(
            $dom->ownerDocument->createElement($name, $value),
            $dom->firstChild
        );

        return simplexml_import_dom($new, __CLASS__);
    }

    public function sort(callable $sorter): void
    {
        if ($this->count() === 0) {
            return;
        }

        $children = iterator_to_array($this->children(), false);
        usort($children, $sorter);

        $dom = dom_import_simplexml($this);

        while ($dom->hasChildNodes()) {
            $dom->removeChild($dom->firstChild);
        }

        foreach ($children as $child) {
            $dom->appendChild(dom_import_simplexml($child));
        }
    }

    public function asXML($filename = null)
    {
        $dom = new DOMDocument("1.0");
        $dom->preserveWhiteSpace = false;
        $dom->formatOutput = true;
        $dom->loadXML(parent::asXML());

        $xml = $dom->saveXML();

        // Go from 2 space indentation to 1.
        $xml = preg_replace('~^([ ]+)\1<(?!/notes>)~m', '\1<', $xml);

        if ($filename === null) {
            return $xml;
        }

        if (file_exists($filename)) {
            rename($filename, $filename . '~');
        }
        file_put_contents($filename, $xml);
    }

    public function xpath($xpath)
    {
        $this->registerXPathNamespace('p', 'http://pear.php.net/dtd/package-2.0');

        return parent::xpath($xpath);
    }
}

function iterable_column(iterable $iterable, $column): Generator
{
    foreach ($iterable as $key => $item) {
        yield $key => $item[$column];
    }
}

function gitignore_filter(iterable $files): Generator
{
    $spec = [
       ["pipe", "r"],
       ["pipe", "w"],
       ["pipe", "w"],
    ];

    $process = proc_open('git check-ignore --non-matching --verbose --stdin', $spec, $pipes);
    stream_set_timeout($pipes[1], 1);
    stream_set_blocking($pipes[2], false);

    foreach ($files as $file) {
        fwrite($pipes[0], $file . "\n");
        $line = fgets($pipes[1], 1024);

        if ($line === false || substr($line, 0, 2) === '::') {
            yield $file;
        }
    }

    $err = fread($pipes[2], 10000);

    fclose($pipes[0]);
    fclose($pipes[1]);
    fclose($pipes[2]);

    $ret = proc_close($process);

    if ($ret !== 0 && $err !== '') {
        fwrite(STDERR, "\033[0;31m" /* red */ . $err . "\033[0m" /* no color */ . "\n");
        throw new RuntimeException("Checking .gitignore failed");
    }
}

