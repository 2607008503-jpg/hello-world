#include <stdio.h>                                      
#include <string.h>                                     // 使用 strcmp、strcpy 等字符串功能
#include <stdlib.h>                                     // 使用 atoi 把文字变成整数
#include <time.h>                                       // 使用 time 获取结账时间

#define GOODS 3                                         // 商品一共有 3 种

char name[GOODS][20] = {"cola", "lollipop", "noodles"}; // 三种商品的名字
char code[GOODS][4] = {"001", "002", "003"};          // 三种商品的条码
double price[GOODS] = {3.50, 0.50, 6.00};               // 三种商品的单价
int cart[GOODS] = {0, 0, 0};                            // 购物车中三种商品的数量
int today = 1;                                          // 当前是第几个营业日
int next_number = 1;                                    // 下一张小票的流水号

int find_good (char input [] )      {                   // 根据条码寻找商品
    int i;                                              // i 用来依次检查三种商品
    for (i = 0; i < GOODS; i++) {                       // 从第 1 种商品检查到第 3 种
        if (strcmp(input, code[i]) == 0) return i;       // 条码相同就返回商品位置
    }                                                   // 商品检查结束
    return -1;                                          // -1 表示没有找到商品
}                                                       // find_good 函数结束

void show_prices(void) {                                // 显示全部商品
    int i;                                              // i 用来遍历商品
    printf("Item\t\tNo.\tPrice\n");                   // 打印表头
    printf("--------------------------------\n");       // 打印分隔线
    for (i = 0; i < GOODS; i++) {                       // 依次打印三种商品
        printf("%-12s\t%s\t%.2f\n", name[i], code[i], price[i]); // 打印名字、条码和价格
    }                                                   // 商品打印结束
}                                                       // show_prices 函数结束

double print_receipt(void) {                            // 打印当前小票并返回总价
    int i;                                              // i 用来遍历商品
    double amount;                                      // amount 保存一种商品的小计
    double total = 0;                                   // total 保存全部商品的总价
    printf("Receipt\nItem\t\tPrice\tQty\tAmount\n");  // 打印小票表头
    printf("----------------------------------------\n"); // 打印分隔线
    for (i = 0; i < GOODS; i++) {                       // 依次检查三种商品
        if (cart[i] > 0) {                              // 只打印数量大于 0 的商品
            amount = price[i] * cart[i];                // 单价乘数量得到小计
            total = total + amount;                     // 把小计加入总价
            printf("%-12s\t%.2f\tx%d\t%.2f\n", name[i], price[i], cart[i], amount); // 打印一行商品
        }                                               // 这个商品处理结束
    }                                                   // 三种商品检查结束
    printf("----------------------------------------\n"); // 打印分隔线
    printf("Total\t\t\t\t%.2f\n", total);            // 打印总价
    return total;                                       // 把总价交给调用这个函数的地方
}                                                       // print_receipt 函数结束

void clear_cart(void) {                                 // 清空购物车
    int i;                                              // i 用来遍历商品
    for (i = 0; i < GOODS; i++) cart[i] = 0;            // 把每种商品的数量都改成 0
}                                                       // clear_cart 函数结束

void load_data(void) {                                  // 启动时读取营业日和流水号
    FILE *file;                                         // file 是文件指针
    int number, day, q1, q2, q3;                        // 保存从销售文件读出的整数
    char date[11], clock_text[9];                       // 保存日期和时间
    double total;                                       // 保存文件中的总价
    file = fopen("day.txt", "r");                      // 尝试打开营业日文件
    if (file != NULL) {                                 // 文件存在才读取
        if (fscanf(file, "%d", &today) != 1) today = 1; // 读取失败就回到第 1 天
        fclose(file);                                   // 读取完成后关闭文件
    }                                                   // 营业日读取结束
    file = fopen("sales.txt", "r");                    // 尝试打开历史销售文件
    if (file != NULL) {                                 // 文件存在才读取
        while (fscanf(file, "%d %d %10s %8s %d %d %d %lf", &number, &day, date, clock_text, &q1, &q2, &q3, &total) == 8) { // 每次读取一条记录
            if (number >= next_number) next_number = number + 1; // 找出下一个流水号
        }                                               // 历史记录读取结束
        fclose(file);                                   // 读取完成后关闭文件
    }                                                   // 销售文件读取结束
}                                                       // load_data 函数结束

void checkout(void) {                                   // 结账并保存销售记录
    FILE *file;                                         // file 用来操作销售文件
    time_t now;                                         // now 保存当前时间
    struct tm *local;                                   // local 保存拆开的年月日和时分秒
    char date[11], clock_text[9];                       // 保存格式化后的日期和时间
    double total = print_receipt();                     // 打印小票并取得总价
    if (total == 0) {                                   // 总价为 0 说明购物车为空
        printf("Cart is empty.\n");                    // 告诉用户不能结账
        return;                                         // 直接结束本次结账
    }                                                   // 空购物车检查结束
    now = time(NULL);                                   // 获取电脑当前时间
    local = localtime(&now);                            // 把当前时间拆成年月日等部分
    strftime(date, sizeof(date), "%Y-%m-%d", local);   // 得到 2026-09-19 这样的日期
    strftime(clock_text, sizeof(clock_text), "%H:%M:%S", local); // 得到 12:30:05 这样的时间
    file = fopen("sales.txt", "a");                    // 用追加方式打开文件，不覆盖旧记录
    if (file == NULL) {                                 // 文件打开失败时
        printf("ERROR: cannot save sales.\n");         // 告诉用户保存失败
        return;                                         // 不清空购物车，方便重新结账
    }                                                   // 文件错误处理结束
    fprintf(file, "%d %d %s %s %d %d %d %.2f\n", next_number, today, date, clock_text, cart[0], cart[1], cart[2], total); // 写入一条销售记录
    fclose(file);                                       // 保存完成后关闭文件
    printf("Saved as sale No.%d.\n", next_number);     // 告诉用户流水号
    next_number++;                                      // 下一张小票的流水号加 1
    clear_cart();                                       // 成功结账后清空购物车
}                                                       // checkout 函数结束

void show_sales(int wanted_day) {                       // 查看指定营业日的销售记录
    FILE *file;                                         // file 用来读取销售文件
    int number, day, qty[GOODS], i, found = 0;           // 保存流水号、营业日、数量和是否找到记录
    char date[11], clock_text[9];                       // 保存日期和时间
    double total, daily_total = 0;                      // 保存单笔总价和全天营业额
    file = fopen("sales.txt", "r");                    // 用只读方式打开销售文件
    printf("Sales of day %d\n", wanted_day);            // 显示正在查询哪一天
    if (file == NULL) {                                 // 没有销售文件说明还没有结过账
        printf("No sales record.\nDaily: 0.00\n");      // 显示没有记录
        return;                                         // 结束查询
    }                                                   // 无文件处理结束
    while (fscanf(file, "%d %d %10s %8s %d %d %d %lf", &number, &day, date, clock_text, &qty[0], &qty[1], &qty[2], &total) == 8) { // 逐条读取记录
        if (day == wanted_day) {                        // 只显示用户想看的营业日
            printf("No.%d  %s %s\n", number, date, clock_text); // 打印流水号和时间
            for (i = 0; i < GOODS; i++) {               // 依次检查三种商品
                if (qty[i] > 0) printf("  %s x%d\n", name[i], qty[i]); // 打印买过的商品
            }                                           // 商品明细打印结束
            printf("  Total: %.2f\n", total);           // 打印这一单的总价
            daily_total = daily_total + total;          // 把这一单加入全天营业额
            found = 1;                                  // 记住已经找到记录
        }                                               // 营业日判断结束
    }                                                   // 销售文件读取结束
    fclose(file);                                       // 读取完成后关闭文件
    if (found == 0) printf("No sales record.\n");       // 没找到时给出提示
    printf("Daily: %.2f\n", daily_total);               // 打印当天营业额
}                                                       // show_sales 函数结束

void start_new_day(void) {                              // 开始新的营业日
    FILE *file;                                         // file 用来保存营业日
    today++;                                            // 营业日编号加 1
    file = fopen("day.txt", "w");                      // 打开文件并写入新的营业日
    if (file == NULL) {                                 // 文件打开失败时
        today--;                                        // 恢复原来的营业日
        printf("ERROR: cannot save new day.\n");        // 告诉用户保存失败
        return;                                         // 结束操作
    }                                                   // 文件错误处理结束
    fprintf(file, "%d\n", today);                      // 保存新的营业日编号
    fclose(file);                                       // 保存完成后关闭文件
    clear_cart();                                       // 新的一天同时清空购物车
    printf("New day started. Today is day %d.\n", today); // 告诉用户操作成功
}                                                       // start_new_day 函数结束

int main(void) {                                        // 程序从 main 函数开始运行
    char input[50];                                     // input 保存用户输入的一整行命令
    char words[10][50], *piece;                         // words 保存一行中最多十段内容
    int parts, i, place;                                // 保存命令段数、循环位置和商品位置
    load_data();                                        // 启动时读取以前的数据
    printf("Commands: code, -code, prices, print, drop, checkout, sales [day], newday, exit\n"); // 显示帮助
    while (1) {                                         // 一直运行，直到用户输入退出命令
        printf("> ");                                  // 提示用户输入
        if (fgets(input, sizeof(input), stdin) == NULL) break; // 读取一整行，读取失败就退出
        input[strcspn(input, "\n")] = '\0';            // 去掉输入末尾的换行符
        parts = 0;                                      // 刚开始还没有拆出任何一段
        piece = strtok(input, " ");                     // 取出空格前的第一段
        while (piece != NULL && parts < 10) {           // 最多取出十段内容
            strcpy(words[parts++], piece);              // 保存这一段，然后让段数加 1
            piece = strtok(NULL, " ");                  // 继续取出下一段
        }                                               // 一整行拆分结束
        if (parts < 1) continue;                        // 空行不做任何事情
        if (strcmp(words[0], "exit") == 0 || strcmp(words[0], "quit") == 0) break; // 输入 exit 或 quit 就退出
        if (strcmp(words[0], "prices") == 0) { show_prices(); continue; } // prices 显示价目表
        if (strcmp(words[0], "print") == 0) { print_receipt(); continue; } // print 打印当前小票
        if (strcmp(words[0], "drop") == 0) { clear_cart(); printf("Cart cleared.\n"); continue; } // drop 清空购物车
        if (strcmp(words[0], "checkout") == 0) { checkout(); continue; } // checkout 结账
        if (strcmp(words[0], "newday") == 0) { start_new_day(); continue; } // newday 开始新的一天
        if (strcmp(words[0], "sales") == 0) {            // sales 用来查询销售记录
            if (parts == 1) show_sales(today);           // 没写日期就查询今天
            else show_sales(atoi(words[1]));             // 写了日期就查询指定营业日
            continue;                                    // 查询结束后等待下一条命令
        }                                                // sales 命令处理结束
        for (i = 0; i < parts; i++) {                    // 依次处理这一行的所有商品条码
            char *current = words[i];                    // current 指向现在要处理的条码
            int minus = (current[0] == '-');             // 条码前有减号表示删除一件
            place = find_good(minus ? current + 1 : current); // 去掉减号后寻找商品
            if (place == -1) printf("ERROR: code not found.\n"); // 没找到商品就报错
            else if (minus && cart[place] == 0) printf("ERROR: item is not in cart.\n"); // 不能把数量减成负数
            else {                                       // 条码正确而且可以修改数量时
                cart[place] = cart[place] + (minus ? -1 : 1); // 根据命令增加或减少一件
                printf("%s %.2f x%d\n", name[place], price[place], cart[place]); // 显示修改后的数量
            }                                            // 单个条码处理结束
        }                                                // 本行条码处理结束
    }                                                    // 主循环结束
    return 0;                                            // 告诉系统程序正常结束
}                                                        // main 函数结束
