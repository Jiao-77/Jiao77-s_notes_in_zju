#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EOR1 "<eor>"
#define EOR2 "<EOR>"
#define EOH "<EOH>"

// 定义一个结构体，用来存储每条数据的字段
typedef struct _record{
	char qso_date[9];
	char time_on[7];
	char freq[11];
	char mode[6];
	char call[16];
	char rst_rcvd[4];
	char rst_sent[4]; 
} record;

int list(char *start_time, char *end_time);
int ends_with(char *str, char *suffix);
int read_adif(FILE *fp, record *r);
int read_csv(FILE *fp, record *r);
int write_adif(FILE *fp, record *r);
int write_csv(FILE *fp, record *r);
int compare(record *r1, record *r2);
int import(char *file_name);
int export(char *file_name);
int search(char *call);
char *f_gets(char *s, int size, FILE *stream);

// 定义一个全局变量，用来存储自己的二进制数据文件的名字
char *data_file = "data.bin";

// 定义一个函数，用来判断一个字符串是否以某个后缀结尾
int ends_with(char *str, char *suffix) {
	int len_str = strlen(str);
	int len_suffix = strlen(suffix);
	if (len_str < len_suffix) return 0;
	return strcmp(str + len_str - len_suffix, suffix) == 0; // 比较字符串末尾和后缀是否相同，返回结果（真或假）
}
// 定义一个函数，模拟fgets，但是以<eor>或<EOR>结束读入
char *f_gets(char *s, int size, FILE *stream) {
  if (s == NULL || size <= 0 || stream == NULL) {
    return NULL;
  }
  char buffer[size];
  buffer[0] = '\0';
  char *end = buffer;
  int found = 0;
  while (!feof(stream) && !ferror(stream) && end - buffer < size - 1 && !found) {
    char c = fgetc(stream);
    if (c != EOF) {
      *end++ = c;
      *end = '\0';
      if (strlen(buffer) >= strlen(EOR1) && strcmp(end - strlen(EOR1), EOR1) == 0) {
        end -= strlen(EOR1);
        *end = '\0';
        found = 1;
      } else if (strlen(buffer) >= strlen(EOR2) && strcmp(end - strlen(EOR2), EOR2) == 0) {
        end -= strlen(EOR2);
        *end = '\0';
        found = 1;
      }
    }
	else return NULL;
  }
  if (buffer[0] != '\0') {
    strcpy(s, buffer);
    return s;
  }
  return NULL;
}

// 定义一个函数，用来从ADIF文件中读取一条数据，并存储在结构体中

int read_adif(FILE *fp, record *r) {
	char line[1024];
	char *p;
	int n;
	
	r->qso_date[0] = '\0';
	r->time_on[0] = '\0';
	r->freq[0] = '\0';
	r->mode[0] = '\0';
	r->call[0] = '\0';
	r->rst_rcvd[0] = '\0';
	r->rst_sent[0] = '\0';
	
	if (f_gets(line, sizeof(line), fp) == NULL) return -1;
	
	p = strstr(line, "<QSO_DATE:");
	if (p != NULL) {
		p += 10;
		n = atoi(p);
		p = strchr(p, '>');
		if (p != NULL && n == 8) {
			p++;
			strncpy(r->qso_date, p, n);
			r->qso_date[n] = '\0';
		}
	}
	
	// 查找time_on字段，方法同上
	p = strstr(line, "<TIME_ON:");
	if (p != NULL) {
		p += 9;
		n = atoi(p);
		p = strchr(p, '>');
		if (p != NULL && (n == 4 || n == 6)) {
			p++;
			strncpy(r->time_on, p, n);
			r->time_on[n] = '\0';
			if (n == 4) {
				strcat(r->time_on, "00");
			}
		}
	}
	
	p = strstr(line, "<FREQ:");
	if (p != NULL) {
		p += 6;
		n = atoi(p);
		p = strchr(p, '>');
		if (p != NULL && n <= 10) {
			p++;
			strncpy(r->freq, p, n);
			r->freq[n] = '\0';
		}
	}
	
	p = strstr(line, "<MODE:");
	if (p != NULL) {
		p += 6;
		n = atoi(p);
		p = strchr(p, '>');
		if (p != NULL && n <= 5) {
			p++;
			strncpy(r->mode, p, n);
			r->mode[n] = '\0';
		}
	}
	
	p = strstr(line, "<CALL:");
	if (p != NULL) {
		p += 6;
		n = atoi(p);
		p = strchr(p, '>');
		if (p != NULL && n <= 15) {
			p++;
			strncpy(r->call, p, n);
			r->call[n] = '\0';
		}
	}
	
	p = strstr(line, "<RST_RCVD:");
	if (p != NULL) {
		p += 10;
		n = atoi(p);
		p = strchr(p, '>');
		if (p != NULL && n <= 3) {
			p++;
			strncpy(r->rst_rcvd, p, n);
			r->rst_rcvd[n] = '\0';
		}
	}
	
	p = strstr(line, "<RST_SENT:");
	if (p != NULL) {
		p += 10;
		n = atoi(p);
		p = strchr(p, '>');
		if (p != NULL && n <= 3) {
			p++;
			strncpy(r->rst_sent, p, n);
			r->rst_sent[n] = '\0';
		}
	}
	
	return 0;
}

// 定义一个函数，用来从CSV文件中读取一条数据，并存储在结构体中
int read_csv(FILE *fp, record *r) {
	char line[256];
	char *p;
	
	r->qso_date[0] = '\0';
	r->time_on[0] = '\0';
	r->freq[0] = '\0';
	r->mode[0] = '\0';
	r->call[0] = '\0';
	r->rst_rcvd[0] = '\0';
	r->rst_sent[0] = '\0';
	
	if (fgets(line, sizeof(line), fp) == NULL) return -1;
	
	p = strtok(line, ",");
	if (p != NULL && strlen(p) == 8) {
		strcpy(r->qso_date, p);
	}
	
	p = strtok(NULL, ",");
	if (p != NULL && (strlen(p) == 4 || strlen(p) == 6)) {
		strcpy(r->time_on, p);
		if (strlen(p) == 4) {
			strcat(r->time_on, "00");
		}
	}
	
	p = strtok(NULL, ",");
	if (p != NULL && strlen(p) <= 10) {
		strcpy(r->freq, p);
	}
	
	p = strtok(NULL, ",");
	if (p != NULL && strlen(p) <= 5) {
		strcpy(r->mode, p);
	}
	
	p = strtok(NULL, ",");
	if (p != NULL && strlen(p) <= 15) {
		strcpy(r->call, p);
	}
	
	p = strtok(NULL, ",");
	if (p != NULL && strlen(p) <= 3) {
		strcpy(r->rst_rcvd, p);
	}
	
	p = strtok(NULL, "\n");
	if (p != NULL && strlen(p) <= 3) {
		strcpy(r->rst_sent, p);
	}
	
	return 0;
}

// 定义一个函数，用来把结构体中的一条数据写入ADIF文件中
int write_adif(FILE *fp, record *r) {
	
	fprintf(fp, "<QSO_DATE:8>%s", r->qso_date);
	
	fprintf(fp, "<TIME_ON:4>%s", r->time_on);
	
	fprintf(fp, "<FREQ:%d>%s", strlen(r->freq), r->freq);
	
	fprintf(fp, "<MODE:%d>%s", strlen(r->mode), r->mode);
	
	fprintf(fp, "<CALL:%d>%s", strlen(r->call), r->call);
	
	fprintf(fp, "<RST_RCVD:%d>%s", strlen(r->rst_rcvd), r->rst_rcvd);
	
	fprintf(fp, "<RST_SENT:%d>%s", strlen(r->rst_sent), r->rst_sent);
	
	fprintf(fp,"<EOR>\n");

	return 0;
}

// 定义一个函数，用来把结构体中的一条数据写入CSV文件中
int write_csv(FILE *fp, record *r) {
	
	fprintf(fp, "%s,", r->qso_date);
	
	fprintf(fp, "%s,", r->time_on);
	
	fprintf(fp, "%s,", r->freq);
	
	fprintf(fp, "%s,", r->mode);
	
	fprintf(fp, "%s,", r->call);
	
	fprintf(fp, "%s,", r->rst_rcvd);
	
	fprintf(fp, "%s\n", r->rst_sent);
	
	return 0;
}

// 定义一个函数，用来比较两个结构体中的数据是否相同
int compare(record *r1, record *r2) {
	return strcmp(r1->qso_date, r2->qso_date) == 0 && strcmp(r1->time_on, r2->time_on) == 0;
}

/* 定义一个函数，用来导入ADIF或CSV文件，根据文件名后缀来决定导入的文件类型
 所导入的数据用以更新自己的二进制数据文件
 导入数据时，不同的数据作为新数据加入，相同的数据则用以更新已有的数据*/
int import(char *file_name) {
	FILE *fp_in;
	FILE *fp_out;
	record r_in;
	record r_out;
	int found; 
	
	// 用一个变量存储文件类型，避免重复判断
	int file_type;
	if (ends_with(file_name, ".adi")) {
		file_type = 1;
	} else if (ends_with(file_name, ".csv")) {
		file_type = 2;
	} else { 
		printf("不支持的文件类型%s\n", file_name);
		return -1;
	}
	
	fp_in = fopen(file_name, "a+");
	if (fp_in == NULL) {
		printf("无法打开文件%s\n", file_name);
		return -1;
	}
	
	fp_out = fopen(data_file, "a+");
	if (fp_out == NULL) {
		printf("无法打开文件%s\n", data_file);
		return -1;
	}
	int tag = 1;
	char ch;
	if(file_type == 1){
		int begin = 0;
		while(!begin){
			while((ch = fgetc(fp_in)) != '<' && ch != EOF);
			begin = 1;
			for(int i = 0;i < 5;i ++){
				ch=(ch >= 'a' && ch <= 'z') ? ch-'a'+'A' : ch;
				if(ch != EOH[i]){
					begin = 0;
					break;
				}
				ch=fgetc(fp_in);
			}
		}
	}
	else if(file_type == 2)
		while(ch = fgetc(fp_in) != '\n' && ch != EOF);
	while (tag) {
		// 用一个switch语句代替if-else语句，提高可读性
		switch (file_type) {
		case 1:
			if (read_adif(fp_in, &r_in) == -1) tag = 0;
			break;
		case 2:
			if (read_csv(fp_in, &r_in) == -1) tag = 0;
			break;
		default:
			break;
		}
		if(tag == 0)break;
		found = 0;
		fseek(fp_out, 0, SEEK_SET);
		while (fread(&r_out, sizeof(record), 1, fp_out) == 1) { 
			if (compare(&r_in, &r_out)) { 
				found = 1;
				fseek(fp_out, -(long)sizeof(record), SEEK_CUR); 
				fwrite(&r_in, sizeof(record), 1, fp_out); 
				break; 
			}
		}
		
		if (!found) {
			fseek(fp_out, 0, SEEK_END);
			fwrite(&r_in, sizeof(record), 1, fp_out); 
			}
	}
	
	fclose(fp_in);
	fclose(fp_out);
	
	return 0; 
}

// 定义一个函数，用来导出ADIF或CSV文件，根据文件名后缀来决定导出的文件类型
int export(char *file_name) {
	FILE *fp_in;
	FILE *fp_out;
	record r;
	
	if (ends_with(file_name, ".adi")) {
		fp_out = fopen(file_name, "w");
		if (fp_out == NULL) {
			printf("无法打开文件%s\n", file_name);
			return -1;
		}
	} else if (ends_with(file_name, ".csv")) {
		fp_out = fopen(file_name, "w");
		if (fp_out == NULL) {
			printf("无法打开文件%s\n", file_name);
			return -1;
		}
	} else {
		printf("不支持的文件类型%s\n", file_name);
		return -1;
	}
	
	fp_in = fopen(data_file, "rb");
	if (fp_in == NULL) {
		printf("无法打开文件%s\n", data_file);
		return -1;
	}
	
	if (ends_with(file_name, ".adi"))fprintf(fp_out,"<EOH>\n");
	else if (ends_with(file_name, ".csv"))fprintf(fp_out,"QSO_DATE,TIME_ON,FREQ,MODE,CALL,RST_RCVD,RST_SENT\n");
	while (fread(&r, sizeof(record), 1, fp_in) == 1) {
		if (ends_with(file_name, ".adi")) { 
			write_adif(fp_out, &r);
		} else if (ends_with(file_name, ".csv")) {
			write_csv(fp_out, &r);
		}
	}
	
	fclose(fp_in);
	fclose(fp_out);
	
	return 0;
}

// 定义一个函数，用来按照call来搜索出有这个call的记录，直接输出为CSV格式
int search(char *call) {
	FILE *fp;
	record r;
	
	fp = fopen(data_file, "rb");
	if (fp == NULL) {
		printf("无法打开文件%s\n", data_file);
		return -1;
	}
	
	while (fread(&r, sizeof(record), 1, fp) == 1) {
		if (strcmp(r.call, call) == 0) {
			write_csv(stdout, &r);
		}
	}
	
	fclose(fp);
	
	return 0;
}

/* 定义一个函数，用来按照时间段来搜索出在这个时间段内的全部记录，直接输出为CSV格式
 时间按照YYYYMMDDhhmmss表示*/
int list(char *start_time, char *end_time) {
	FILE *fp;
	record r;
	char datetime[15];
	
	fp = fopen(data_file, "rb");
	if (fp == NULL) {
		printf("无法打开文件%s\n", data_file);
		return -1;
	}
	
	while (fread(&r, sizeof(record), 1, fp) == 1) {
		strcpy(datetime, r.qso_date);
		strcat(datetime, r.time_on);
		if (strcmp(datetime, start_time) >= 0 && strcmp(datetime, end_time) <= 0) {
			write_csv(stdout, &r);
		}
	}
	
	fclose(fp);
	
	return 0;
}

int main(int argc,char *argv[]) {
	if(argc==1)printf("请输入命令行命令");
	else if (strcmp(argv[1], "-i") == 0) {
		import(argv[2]);
	}
	
	else if (strcmp(argv[1], "-o") == 0) {
		export(argv[2]);
	}
	
	else if (strcmp(argv[1], "-s") == 0) {
		search(argv[2]);
	}
	
	else if (strcmp(argv[1], "-l") == 0) {
		list(argv[2], argv[3]);
	}
	
	else printf("不支持的参数%s\n", argv[1]);
	return -1;
}
