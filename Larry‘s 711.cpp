//1.头文件
#include <algorithm>//算法
#include <chrono>//时间
#include <cctype>//字符type
#include <fstream>//读文件
#include <iomanip>//输出格式
#include <iostream>//输入输出
#include <limits>//数值限制
#include <map>//对数据操作
#include <sstream>//拼字符串
#include <string>//字符串type
#include <vector>//动态数组
//2，定义数据结构
struct Item {std::string code,name;long long price;int stock;};
struct SaleLine {std::string code,name;long long price;int qty;};//销售中一行
struct Sale{int day,id;std::string date,time;std::vector <SaleLine> lines;long long total;};//销售明细
//3.
std::map<std::string, Item> items;//对应查找商品
std::map<std::string,int> cart;//购物车
int currentDay = 1;
int nextid = 1;
bool admin = false;//默认非管理员
//4.money函数
std::string money(long long cents) {
    std::ostringstream out;
    out << cents / 100 << '.' << std::setfill('0') << std::setw(2) << cents % 100;
    return out.str();
}

