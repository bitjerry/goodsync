//
// Created by Mr.lin on 2024-04-01.
//

#include <Python.h>
#include <stdio.h>
#include "goodsync.h"

static PyObject *GSError;

static PyObject *encrypt_master_key(PyObject *self, PyObject *args, PyObject *keywds) {
    unsigned char *master_key;
    char *keys[] = {"master_key", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "s", keys, &master_key)) {
        return NULL;
    }
    unsigned char out[1024] = {0};
    int out_len;
    int result = aes_encrypt_master_key(master_key, sizeof(master_key) - 1, out, &out_len);
    if (result) {
        PyErr_SetString(GSError, message(result));
        return NULL;
    }
    return PyUnicode_FromStringAndSize((char *) out, out_len);
}


static PyObject *encrypt_name(PyObject *self, PyObject *args, PyObject *keywds) {
    unsigned char *key;
    unsigned char *data;
    char *keys[] = {"key", "data", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ss", keys, &key, &data)) {
        return NULL;
    }
    unsigned char out[1024] = {0};
    int out_len;
    int result = aes_encrypt_name(key, sizeof(key) - 1, data, sizeof(data) - 1, out, &out_len);
    if (result) {
        PyErr_SetString(GSError, message(result));
        return NULL;
    }
    return PyUnicode_FromStringAndSize((char *) out, out_len);
}

static PyObject *decrypt_name(PyObject *self, PyObject *args, PyObject *keywds) {
    unsigned char *key;
    unsigned char *data;
    char *keys[] = {"key", "data", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ss", keys, &key, &data)) {
        return NULL;
    }
    unsigned char out[1024] = {0};
    int out_len;
    int result = aes_decrypt_name(key, sizeof(key) - 1, data, sizeof(data) - 1, out, &out_len);
    if (result) {
        PyErr_SetString(GSError, message(result));
        return NULL;
    }
    out[out_len] = 0;
    return PyUnicode_FromStringAndSize((char *) out, out_len);
}

static PyMethodDef methods[] = {
        {"encrypt_master_key", (PyCFunction) encrypt_master_key,
                                                           METH_VARARGS |
                                                           METH_KEYWORDS, "Encrypt plaintext master passwords of any length"},
        {"encrypt_name",       (PyCFunction) encrypt_name, METH_VARARGS |
                                                           METH_KEYWORDS, "Generate GoodSync encrypted filename using encrypted master password"},
        {"decrypt_name",       (PyCFunction) decrypt_name, METH_VARARGS |
                                                           METH_KEYWORDS, "Decrypt GoodSync filename using encrypted master password"},
        {NULL, NULL,                                       0, NULL}
};

static PyModuleDef module = {
        PyModuleDef_HEAD_INIT,
        "GoodSync",
        "Fuck GoodSync Filename",
        -1,
        methods,
        NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC PyInit__GoodSync(void) {
    PyObject *m = PyModule_Create(&module);
    if (m == NULL) {
        return NULL;
    }
    GSError = PyErr_NewException("goodsync.GSError", NULL, NULL);
    Py_XINCREF(GSError);
    if (PyModule_AddObject(m, "error", GSError) < 0) {
        Py_XDECREF(GSError);
        Py_CLEAR(GSError);
        Py_DECREF(m);
        return NULL;
    }
    return m;
}
