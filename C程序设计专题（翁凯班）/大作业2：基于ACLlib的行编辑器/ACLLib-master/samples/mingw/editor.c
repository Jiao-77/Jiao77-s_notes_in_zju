#include "acllib.h"
#include <string.h>
#include <stdio.h>

void print(char c);
void key(int key,int ev);

typedef struct _node 
{
	char ch;
	struct _node* next;
	struct _node* prev;
}Node;

Node *head = NULL,*tail = NULL, *node_ = NULL;
int  care = 2, careposition = 0, flag = 0 , insert = 0;

int Setup()
{
	initConsole();
	initWindow("editor",DEFAULT,DEFAULT,900,500);
	head = (Node*)malloc(sizeof(Node));
	head->next = NULL;
	head->prev = NULL;
	head->ch = 0;
	node_ = head;

	beginPaint();
	setCaretSize(care,25);
	setCaretPos(0,0);
	showCaret();
	setTextColor(BLACK);
	setTextSize(18);
	registerKeyboardEvent(key);
	if(!flag){
		registerCharEvent(print);
	}
	endPaint();
	flag = 0;
	return 0;
}

void print(char c)
{
	register unsigned int i = 0,j = 0; //使用寄存器变量和无符号整型
	Node *p = NULL,*q = NULL;
	
	beginPaint();
	clearDevice();
	if(c == 8){//shanchu
		if(node_ != head){
			node_->prev->next = node_->next;
			if(node_->next)
				node_->next->prev = node_->prev;
			p = node_;
			node_ = p->prev;
			free(p);
			if(!(node_->next)) tail = node_;
		}
	}
	else if(c<32||c==127); 
	else{
			if(!(head->next)){
				head->next = (Node*)malloc(sizeof(Node));
				tail = head->next;
				head->next->prev = head;
				head->next->next= NULL;
				tail->ch = c;
				node_ = tail;//³õÊ¼»¯ 
			}
			else if(node_ == tail){
				p = (Node*)malloc(sizeof(Node));
				tail->next = p;
				p->next = NULL;
				p->prev = tail;
				tail = p;
				tail->ch = c;
				node_ = node_->next;
			}
			else{
				if(care == 2){
					p = (Node*)malloc(sizeof(Node));
					p->next = node_->next;
					node_->next->prev = p;
					node_->next = p;
					p->prev = node_;
					node_ = p;
					node_->ch = c;
				}
				else{
					node_->next->ch = c;
					node_ = node_->next;
				}
			}
	}
	p = head->next;
	char s[2] = {0};
	static char *classes="WSU";
	while(p){
		s[0] = p->ch;
		paintText(i*10,0,s);
		i++;
		if(p == node_) j = i;
		p = p->next;
	}
	careposition = j*10; //使用移位操作代替乘法运算
	setCaretPos(careposition,0);
	showCaret();
	endPaint();	

}

void key(int key,int ev)
{
	Node *p = NULL;
	char x[2] = {0};
	if(ev == 0){ //按键按下
		flag = 1;
		switch(key){
			case 37: //左
				if(node_->prev){
					node_ = node_->prev;
					careposition-=10;
					setCaretPos(careposition,0);
					showCaret();
				}
				break;
			case 39: //右
				if(node_->next){
					node_ = node_->next;
					careposition+=10;
					setCaretPos(careposition,0);
					showCaret();
				}
				break;
		}
	}
	else if(ev == 1){ //按键释放
		flag = 1;
		switch(key){
			case 46: //删除
				if(node_->next){
					p = node_->next;
					node_->next = p->next;
					if(p->next)
						p->next->prev = node_;
					free(p);
					if(!(node_->next)) tail = node_; //更新尾指针
				}
				p = head->next;
				beginPaint();
				clearDevice();
				int i = 0;
				while(p){
					x[0] = p->ch;
					x[1] = '\0';
					paintText(11*i,0,x);
					i=i+1; 
					p = p->next;
				}
				showCaret();
				endPaint();
				break;
			case 45: //切换光标大小
				if(care == 2)
					care = 10;
				else care = 2;
				setCaretSize(care,25);
				showCaret();
				break;
			case 13: //回车
				beginPaint();
				p = head->next;
				while(p){
					printf("%c",p->ch);
					p = p->next;
					if(p) free(p->prev);
				}
				free(tail);
				tail = NULL;
				head->next = NULL;
				head->prev = NULL;
				head->ch = 0;
				node_ = head;
				printf("\n");
				clearDevice();	
				careposition = 0;
				setCaretPos(careposition,0);
				showCaret();
				endPaint();
		}
	}
}