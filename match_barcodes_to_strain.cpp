

#include "match_barcodes_to_strain.h"
using namespace std;

int main()
{
HP_ReadTextFile* readfile = new HP_ReadTextFile();
HP_DynamicStringArray* barcodes = new HP_DynamicStringArray();
HPDynamicIntArray* up_down =  new HPDynamicIntArray();
HP_DynamicStringArray* up_codes = new HP_DynamicStringArray();
HPDynamicIntArray* up_ID =  new HPDynamicIntArray();
HP_DynamicStringArray* down_codes = new HP_DynamicStringArray();
HPDynamicIntArray* down_ID =  new HPDynamicIntArray();

char* file_in = new char[1024];


memset(file_in, 0, 1024);
strcpy(file_in, "C:\\Users\\hpatterton\\Documents\\Students\\Mzwanele Ngubo\\PhD\\Sequence data\\Sample_H1\\H1_ATCACG_L001_R1_001_barcodes3.txt");
char* file_extension_dot = strrchr(file_in,'.');
int number_of_characters = file_extension_dot-file_in;
char* file_out = new char[1024];
memset(file_out,0,1024);
strncpy(file_out,file_in,number_of_characters);
strcat(file_out,"_output.txt");

int entries = readfile->ReadBarcodeFile(file_in, barcodes, up_down, "\n");

int entries_up = readfile->ReadBarcodeFile("src\\Up_tags.txt", up_codes, up_ID, "\r\n");
int entries_down = readfile->ReadBarcodeFile("src\\Down_tags.txt", down_codes, down_ID, "\r\n");
int* up_hits = new int[entries_up];
for(int x = 0; x < entries_up; x++)
	up_hits[x] = 0;
int* down_hits = new int[entries_down];
for(int x = 0; x < entries_down; x++)
	down_hits[x] = 0;
int sum_up = 0;
int sum_down = 0;
char* comp1;
char* comp2;
for(int x = 0; x < entries; x++)
	{
	int index;
	if(up_down->GetEntry(x) == 1)
		{
		index = 0;
		comp1 = barcodes->GetStringPointer(x);
		comp2 = up_codes->GetStringPointer(index);
		while((index < entries_up) && (strcmp(comp1, comp2) != 0))
			{
			index++;
			comp2 = up_codes->GetStringPointer(index);
			}
		if(index < entries_up) // we have a match
			up_hits[index]++;
		}
	else
		{
		index = 0;
		comp1 = barcodes->GetStringPointer(x);
		comp2 = down_codes->GetStringPointer(index);
		while((index < entries_down) && (strcmp(comp1, comp2) != 0))
			{
			index++;
			comp2 = down_codes->GetStringPointer(index);
			}
		if(index < entries_down) // we have a match
			down_hits[index]++;
		}
	}
for(int x = 0; x < entries_up; x++)
	sum_up += up_hits[x];
for(int x = 0; x < entries_down; x++)
	sum_down += down_hits[x];
cout << "up: " << sum_up << endl;
cout << "down: " << sum_down << endl;
cout << "Up:Down: " << (float)sum_up/sum_down << endl;

// Write result file to disk

fstream outfile;
outfile.open(file_out, fstream::out | fstream::binary);
char* temp = new char[1024];
for(int x = 0; x < entries_up; x++)
{
	memset(temp, 0, 1024);
	sprintf(temp, "%d%c%d%c%d%c%d", up_ID->GetEntry(x), '\t', up_hits[x], '\t', down_ID->GetEntry(x), '\t', down_hits[x]);
	outfile << temp << endl;
}
outfile.close();
cout << entries_up << " values written to " << file_out << endl;
cout << "done" << endl;

// Reclaim memory

delete [] file_in;
delete [] file_out;
delete [] up_hits;
delete [] down_hits;
delete [] temp;

return 0;
}
