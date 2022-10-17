/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: b8ea2fddf5afdd9d45d2541d63169e659208d20e */




static const zend_function_entry class_SimdJsonException_methods[] = {
	ZEND_FE_END
};


static const zend_function_entry class_SimdJsonValueError_methods[] = {
	ZEND_FE_END
};

static zend_class_entry *register_class_SimdJsonException(zend_class_entry *class_entry_RuntimeException)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "SimdJsonException", class_SimdJsonException_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_RuntimeException);

	return class_entry;
}

static zend_class_entry *register_class_SimdJsonValueError(zend_class_entry *class_entry_ValueError)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "SimdJsonValueError", class_SimdJsonValueError_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_ValueError);

	return class_entry;
}
