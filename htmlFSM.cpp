#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#define BUFFER_SIZE 4096
enum CHECK_STATE
{
    CHECK_STATE_REQUESTLINE = 0,
    CHECK_STATE_HEADER
};
enum LINE_STATUS //从状态机，解析一行
{
    LINE_OK = 0,//
    LINE_BAD,
    LINE_OPEN
};
enum HTTP_CODE
{
    NO_REQUEST,//请求不完整
    GET_REQUEST,//完整的用户请求
    BAD_REQUEST,//请求语法错误
    FORBIDDEN_REQUEST,//客户对资源无访问权限
    INTERNAL_ERROR,//服务器内部错误
    CLOSE_REQUEST//关闭连接
};

static const char *szret[] = {"I get a correct result\n", "Something wrong\n"};

LINE_STATUS parse_line(char *buffer, int &checked_index, int &read_index)
{
    char temp;
    for (; checked_index < read_index;++checked_index)
    {
        temp = buffer[checked_index];
        if(temp=='\r')
        {
            if((checked_index+1)==read_index)
            {
                return LINE_OPEN;
            }
            else if (buffer[checked_index+1]=='\n')
            {
                buffer[checked_index++]='\0';
                buffer[checked_index++]='\0';
                return LINE_OK;
            }
        }
    }
}