#include"unity.h"
#include"netinfo.h"
void setUp(void)
{
}
void tearDown(void)
{
}
void test_GetNetInfoIpFunc(void)
{
    void *data = NULL;
    unsigned int len = 0;
    int ret = GetNetInfoIpFunc(&data, &len);
    TEST_ASSERT_EQUAL_INT(1, ret);
    TEST_ASSERT_EQUAL_INT(9, len);
}