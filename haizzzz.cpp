#include <iostream>
#include <algorithm>
#include <string.h>

using namespace std;

struct Rotation{
	int index;
	char* suffix;
};

bool cmpRotation(Rotation tmp1, Rotation tmp2){
	int cmp = strcmp(tmp1.suffix, tmp2.suffix);
	if(cmp >= 0) return false;
	return true;
}

void rotate(char* &text){
	int n = strlen(text);
	char temp = text[0];
	for(int i = 0; i < n; i++){
		if(i == n-1) text[n-1] = temp;
		else text[i] = text[i+1];
	}
}

int* computeSuffixArrayIndex(char* text, int n, int &k){
	Rotation temp[n];
	int* suffix_arr_index = new int[n];

	//Mảng hậu tố.
	for(int i = 0; i < n; i++){
		temp[i].index = i;
		temp[i].suffix =  new char[n];
		if(i == 0) strcpy(temp[i].suffix, text);
		else {
			//Mỗi lần quay 1 so với xâu quay lần trước.
			rotate(text);
			strcpy(temp[i].suffix, text);
		}
	}

	//Sắp xếp mảng temp theo thứ tự AlphaBeta của phần tử suffix : không hiểu tại sao nó sort() sai 1 cái :))
	sort(temp, temp + n, cmpRotation);

	//Tính suffix_arr_index dựa trên danh sách đã sắp xếp.
	for(int j = 0; j < n; j++){
		suffix_arr_index[j] = temp[j].index;
		if(temp[j].index == 0) k = j; //Hàng chứa xâu gốc theo index 0.
	}

	for(int i = 0; i < n; i++){
		cout << temp[i].suffix << endl;
	}

	rotate(text); //Quay về xâu cũ

	return suffix_arr_index;
}

char* getLastColumn(char* text, int* suffix_arr_index, int n){
	char* bwt = new char[n];
	int index;

	for(int i = 0; i < n; i++){
		index = suffix_arr_index[i];
		if(index == 0) bwt[i] = text[n-1];
		else bwt[i] = text[index - 1];
	}
	return bwt;
}

int findLocation(char* bwt, char ch, int dem){
	char* temp = strchr(bwt, ch);
	for(int i = 0; i < dem - 1; i++){
		temp = strchr(temp + 1, ch);
	}
	return temp - bwt;
}

// Không tổng quát :Fail.
char* inverseBWT(char* bwt, int n, int k){
	//temp1 : xắp sếp bwt theo alpha-beta.
	char temp1[n];
	strcpy(temp1, bwt);
	sort(temp1, temp1 + n);
	//temp2, s : dạng mảng và dạng chuỗi sau khi khôi phục. 
	char* temp2 = new char[n];
	string s;
	char ch = temp1[k];
	temp2[0] = ch;
	s += ch;
	//Đếm số lần đã lấy ra 1 ý tự trong bwt;
	int dem;

	for(int i = 1; i < n; i++){
		dem = count(s.begin(), s.end(), ch);
		if(s.length() == 1) k = findLocation(bwt, ch, 0);
		else k = findLocation(bwt, ch, dem);
		ch = temp1[k];
		temp2[i] = ch;
		s += ch;
	}

	return temp2;
}


int main(){
	char arr[] = "^BANANA|";
	int n = strlen(arr);
	int k;
	// cout << k << endl;
	int* suffix_arr_index = computeSuffixArrayIndex(arr, n, k);
	char* bwt = getLastColumn(arr, suffix_arr_index, n);
	cout << "BWT :" << bwt << endl; 
	char* original = inverseBWT(bwt, n, k);
	cout << "original :" << original<< endl;
	delete []original;
	return 0;
}