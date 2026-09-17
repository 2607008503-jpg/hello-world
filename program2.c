#include<stdio.h>
#include<string.h>
         int main () {
               char name1[]="cola",code1[]="001";
               double price1=3.50;

               char name2[]="lollipop",code2[]="002";
               double price2=0.50;

               char name3[]="noodles",code3[]="003";
               double price3=6.00;
          char input[20];
               
               while (1){
               printf("please input code\n");
                scanf("%s",input);
               if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0)
                {break;}
               if (strcmp(input, "prices") == 0 ) {
                   printf("item-\tno.-\tprice-\n");
                   printf("---\n");
                   printf("%s\t%s\t%.2f\n",name1,code1,price1);
                   printf("%s\t%s\t%.2f\n",name2,code2,price2);
                   printf("%s\t%s\t%.2f\n",name3,code3,price3);
                   continue;
               }
               if (strcmp(input, code1) == 0 ) {
                   printf("%s\t%.2f\n",name1,price1);
                  }else if (strcmp(input, code2) == 0 ) {
                   printf("%s\t%.2f\n",name2,price2);
                  }else if (strcmp(input, code3) == 0 ) {
                   printf("%s\t%.2f\n",name3,price3);
                  }else{
                    printf("ERROR: code not found \n");
                  }
                }
           return 0;
          }