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
	initWindow("txt",DEFAULT,DEFAULT,1350,350);
	int start = 1;
	head = (Node*)malloc(sizeof(Node));
	head->next = NULL;
	head->prev = NULL;
	head->ch = 0;
	node_ = head;

	beginPaint();
	setCaretSize(care,25);
	if (start == 1){
		setCaretPos(0,0);
		showCaret();
		start = 0;
    }
	setTextColor(BLACK);
	setTextSize(20);
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
	int i = 0,j = 0;
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
	while(p){
		s[0] = p->ch;
		paintText(i*11,0,s);
		i++;
		if(p == node_) j = i;
		p = p->next;
	}
	careposition = j*11;
	setCaretPos(careposition,0);
	showCaret();
	endPaint();	

}
void key(int key,int ev)
{
	Node *p = NULL;
	char x[2] = {0};
	if(key == 37&&ev == 0){//zuo
		flag = 1;
		if(node_->prev){
			node_ = node_->prev;
			careposition-=11;
			setCaretPos(careposition,0);
			showCaret();
		}
		else return;
	}
	if(key == 39&&ev == 0){//you
		flag = 1;
		if(node_->next){
			node_ = node_->next;
			careposition+=11;
			setCaretPos(careposition,0);
			showCaret();
	    }
		else return;
	}
	if(key == 46&&ev == 1){//delete
		flag = 1;
		if(node_->next){
			if(node_->next == tail){
				node_->next = NULL;
				free(tail);
				tail = node_;
			}
			else{
				p = node_->next;
				node_->next = node_->next->next;
				node_->next->prev = node_;
				free(p);
			}
		}
		else return;
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
	}
	if(key == 45&&ev == 1){
		flag = 1;
		if(care == 2)
			care = 11;
		else care = 2;
		setCaretSize(care,25);
		showCaret();
	}
	if(key == 13&&ev ==1){
		flag =1;
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
