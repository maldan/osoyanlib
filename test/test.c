#include "../osoyan.h"

void memory_check() {
    if (!MEMORY_IS_FREE) {
        fprintf(stderr, "Memory table is not empty");
        exit(1);
    }
}

void test1() {
    NEW_STRING(X);
    string_put(X, "Hello ");
    string_put(X, "world!");
    string_put(X, "");
    PRINT(X);
    assert(strcmp(X->list, "Hello world!") == 0);
    DESTROY_STRING(X);

    memory_check();
}

void test2() {
    NEW_STRING(X);
    string_put(X, "Hello");
    string_put_char(X, ' ');
    string_put_char(X, 'w');
    string_put_char(X, 'o');
    string_put_char(X, 'r');
    string_put_char(X, 'l');
    string_put_char(X, 'd');
    string_put_char(X, '!');
    assert(strcmp(X->list, "Hello world!") == 0);
    DESTROY_STRING(X);

    memory_check();
}

void test3() {
    NEW_STRING(X);
    string_add(X, "[%d%d]", 5, 10);
    string_add(X, "[%d%d]", 10, 20);
    string_add(X, "");
    assert(strcmp(X->list, "[510][1020]") == 0);
    DESTROY_STRING(X);

    memory_check();
}

void test_frm1() {
    NEW_STRING(X);
    string_put(X, "00001111000011110000111100001111");
    struct String *Y = string_break(X, 8);
    assert(strcmp(Y->list, "00001111\n00001111\n00001111\n00001111") == 0);
    DESTROY_STRING(X);
    DESTROY_STRING(Y);

    memory_check();
}

void test_frm2() {
    NEW_STRING(X);
    string_put(X, "00001111");
    struct String *Y = string_break(X, 8);
    assert(strcmp(Y->list, "00001111") == 0);
    DESTROY_STRING(X);
    DESTROY_STRING(Y);

    memory_check();
}

void test_frm3() {
    NEW_STRING(X);
    string_put(X, "00001111");
    struct String *Y = string_break(X, 16);
    assert(strcmp(Y->list, "00001111") == 0);
    DESTROY_STRING(X);
    DESTROY_STRING(Y);

    memory_check();
}

void test_frm4() {
    NEW_STRING(X);

    // 1
    string_put(X, "00001111");
    struct String *Y = string_break(X, 1);
    assert(strcmp(Y->list, "0\n0\n0\n0\n1\n1\n1\n1") == 0);
    X->length = 0;
    DESTROY_STRING(Y);

    // 2
    string_put(X, "0000\n1111");
    Y = string_break(X, 6);
    assert(strcmp(Y->list, "0000\n1111") == 0);
    X->length = 0;
    DESTROY_STRING(Y);

    DESTROY_STRING(X);

    memory_check();
}

void test_frm5() {
    NEW_STRING(X);
    string_put(X, "00001111");
    struct String *Y = string_break(X, 0);
    assert(strcmp(Y->list, "00001111") == 0);
    DESTROY_STRING(X);
    DESTROY_STRING(Y);

    memory_check();
}

void test_frm6() {
    NEW_STRING(X);

    // 1
    string_put(X, "0000\n1111");
    struct String *Y = string_indent(X, 4);
    assert(strcmp(Y->list, "    0000\n    1111") == 0);
    DESTROY_STRING(Y);
    X->length = 0;

    // 2
    string_put(X, "00001111");
    Y = string_indent(X, 4);
    assert(strcmp(Y->list, "    00001111") == 0);

    DESTROY_STRING(X);
    DESTROY_STRING(Y);

    memory_check();
}

void test_strarr_1() {
    NEW_STRING_ARRAY(X);
    string_array_push(X, "sex");
    string_array_push(X, "rock");
    string_array_push(X, "drag'n'drop");
    string_array_push(X, "");
    PRINT(X);
    assert(X->length == 4);
    assert(strcmp(X->list[0]->list, "sex") == 0);
    assert(strcmp(X->list[1]->list, "rock") == 0);
    assert(strcmp(X->list[2]->list, "drag'n'drop") == 0);
    assert(strcmp(X->list[3]->list, "") == 0);
    DESTROY_STRING_ARRAY(X);

    memory_check();
}

void test_strarr_2() {
    NEW_STRING_ARRAY(X);
    string_array_push(X, "sex");
    string_array_push(X, "rock");
    string_array_push(X, "drag'n'drop");
    assert(X->length == 3);

    struct String *Y1 = string_array_join(X, "");
    assert(strcmp(Y1->list, "sexrockdrag'n'drop") == 0);

    struct String *Y2 = string_array_join(X, "-");
    assert(strcmp(Y2->list, "sex-rock-drag'n'drop") == 0);

    DESTROY_STRING(Y1);
    DESTROY_STRING(Y2);
    DESTROY_STRING_ARRAY(X);
    memory_check();
}

void test_strarr_3() {
    NEW_STRING_ARRAY(X);
    string_array_push(X, "sex");
    string_array_push(X, "rock");
    string_array_push(X, "drag'n'drop");
    assert(X->length == 3);
    string_array_clear(X);
    assert(X->length == 0);
    DESTROY_STRING_ARRAY(X);
    memory_check();
}

int main(int argc, char **argv) {
    MEMORY_INIT;

    if (argc < 2) return 1;
    if (strcmp(argv[1], "string_basic") == 0) {
        test1();
        test2();
        test3();
    } else if (strcmp(argv[1], "string_format") == 0) {
        test_frm1();
        test_frm2();
        test_frm3();
        test_frm4();
        test_frm5();
        test_frm6();
    } else if (strcmp(argv[1], "string_array") == 0) {
        test_strarr_1();
        test_strarr_2();
        test_strarr_3();
    } else if (strcmp(argv[1], "other") == 0) {
        NEW_VECTOR(X, int);
        NEW_BLOB(Y);
        DESTROY_VECTOR(X);
        DESTROY_BLOB(Y);

        EQU_BLOB(X1) = sha1_chars("ss");
        DESTROY_BLOB(X1);

        NEW_FILE_INFO(X2, "Makefile");
        DESTROY_FILE_INFO(X2);

        memory_check();
    } else {
        return 1;
    }

    return 0;
}
