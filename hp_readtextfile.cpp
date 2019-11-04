/*
 * temp.cpp
 *
 *  Created on: 27 Sep 2012
 *      Author: patterh
 */

#include "hp_readtextfile.h"

using namespace std;

HP_ReadTextFile::HP_ReadTextFile()
{
array_of_strings = new HP_DynamicStringArray();
number_of_lines = 0;
}

HP_ReadTextFile::~HP_ReadTextFile()
{
delete array_of_strings;
}

int HP_ReadTextFile::ReadBarcodeFile(char* filename, HP_DynamicStringArray* string_array, HPDynamicIntArray* up_down, const char* delimiter)
{
fstream filestr;
filestr.open(filename, fstream::in | fstream::binary);
filestr.seekg (0, filestr.end);
int length = filestr.tellg();
filestr.seekg (0, filestr.beg);
char* buffer;
buffer = new char[length+1];
memset(buffer, 0, length+1);
filestr.read(buffer, length);
filestr.close();
char* start;
int entry = 0;
start = strtok(buffer,delimiter);
char* temp = new char[30];
char* pointer = 0;
char* tab = 0;
int up;
while (start != 0)
	{
	memset(temp, 0, 30);
	strcpy(temp, start);
	tab = strchr(temp, '\t');
	tab[0] = 0;
	pointer = tab + 1;
	up = atoi(pointer);
	string_array->AddString(temp);
	up_down->AddInt(up);
	entry++;
	start = strtok (0,delimiter);
	}
return(entry);
}

char* HP_ReadTextFile::ReverseComplement(char* sequence)
{
int length = strlen(sequence);
char* temp = new char[length];
strcpy(temp, sequence);
for(int x = 0; x < length; x++)
	{
	if(temp[length - 1 - x] == 'G')
		sequence[x] = 'C';
	if(temp[length - 1 - x] == 'C')
		sequence[x] = 'G';
	if(temp[length - 1 - x] == 'A')
		sequence[x] = 'T';
	if(temp[length - 1 - x] == 'T')
		sequence[x] = 'A';
	}
delete [] temp;
return(sequence);
}

int HP_ReadTextFile::ReadFastqFile(char* filename, HP_DynamicStringArray* string_array)
{
fstream filestr;
filestr.open(filename, fstream::in | fstream::binary);
filestr.seekg (0, filestr.end);
int length = filestr.tellg();
filestr.seekg (0, filestr.beg);
char* buffer;
buffer = new char[length+1];
memset(buffer, 0, length+1);
filestr.read(buffer, length);
filestr.close();
char* start;
int entry = 0;
start = strtok(buffer,"\n");
int line = 0;
while (start != 0)
	{
	if(start[0] == '@')
		line = 1;
	else
		{
		if(line == 1)
			{
			string_array-> AddString(start);
			entry++;
			line = 2;
			}

		}
	start = strtok (0,"\n");
	}
return(entry);

}

int HP_ReadTextFile::ReadNucleosomeDyadFile(char*& buffer, char**& list_of_string_pointers, char* filename)
{
fstream filestr;
filestr.open(filename, fstream::in | fstream::binary);
filestr.seekg (0, filestr.end);
int length = filestr.tellg();
filestr.seekg (0, filestr.beg);
buffer = new char[length+1];
memset(buffer, 0, length+1);
list_of_string_pointers = new char*[length+1];
filestr.read(buffer, length);
filestr.close();
char* start;
int entry = 0;
start = strtok(buffer,"\r\n");
while (start != 0)
	{
	list_of_string_pointers[entry++] = start;
	start = strtok (0,"\r\n");
	}
return(entry);
}

int HP_ReadTextFile::ReadNucleosomeDyadFile(float*& dyad_values, char* filename)
{
fstream filestr;
filestr.open(filename, fstream::in | fstream::binary);
filestr.seekg (0, filestr.end);
int length = filestr.tellg();
filestr.seekg (0, filestr.beg);
char* buffer = new char[length+1];
memset(buffer, 0, length+1);
dyad_values = new float[length];
filestr.read(buffer, length);
filestr.close();
char* start;
int entry = 0;
start = strtok(buffer,"\r\n");
while (start != 0)
	{
	dyad_values[entry++] = atof(start);
	start = strtok (0,"\r\n");
	}
delete [] buffer;
return(entry);
}

int  HP_ReadTextFile::ReadSequenceFile(char*& sequence, char* filename)
{
	fstream filestr;
	filestr.open(filename, fstream::in | fstream::binary);
	filestr.seekg (0, filestr.end);
	int length = filestr.tellg();
	filestr.seekg (0, filestr.beg);
	char* buffer = new char[length+1];
	memset(buffer, 0, length+1);
	sequence = new char[length+1];
	memset(sequence, 0, length+1);
	filestr.read(buffer, length);
	filestr.close();
	char* pointer = strstr(buffer,"\r\n");
	char* start = strtok(pointer,"\r\n");
	while (start != 0)
		{
		strcat(sequence, start);
		start = strtok (0,"\r\n");
		}
	return(strlen(sequence));
}

void HP_ReadTextFile::ReadFastaFile(char* filename)
{
fstream filestr;
filestr.open(filename, fstream::in);
char* temp = 0;
while(filestr.eof() != true)
  {
          temp = new char[1024];
          memset(temp,0,1024);
          filestr.getline(temp,1024);
          array_of_strings->AddString(temp);
          delete [] temp;
  }
}

int HP_ReadTextFile::ReadFileofGenomicPositions(HPDynamicIntArray* chromosome_number, HPDynamicIntArray* genomic_position, HP_DynamicStringArray* strand,  char* filename)
{
fstream filestr;
filestr.open(filename, fstream::in | fstream::binary);
filestr.seekg (0, filestr.end);
int length = filestr.tellg();
filestr.seekg (0, filestr.beg);
char* buffer = new char[length+1];
memset(buffer, 0, length+1);
filestr.read(buffer, length);
filestr.close();
char* start;;
char* second_entry;
char* third_entry;
int entry = 0;
start = strtok(buffer,"\r\n");
while (start != 0)
	{
	chromosome_number->AddInt(atoi(start));
	second_entry = strchr(start,'\t');
	genomic_position->AddInt(atoi(second_entry+1));
	third_entry = strchr(second_entry+1, '\t');
	strand->AddString(third_entry+1);
	start = strtok (0,"\r\n");
	entry++;
	}
delete [] buffer;
return(entry);

}



int HP_ReadTextFile::ReadTextFile(char*& buffer, char**& list_of_string_pointers, char* filename)
{
fstream filestr;
filestr.open(filename, fstream::in | fstream::binary);
filestr.seekg (0, filestr.end);
int length = filestr.tellg();
filestr.seekg (0, filestr.beg);
buffer = new char[length+1];
memset(buffer, 0, length+1);
list_of_string_pointers = new char*[length+1];
/*int entry = 0;
char* temp = 0;
while(!filestr.eof())
{
	temp = new char[30];
	filestr.getline(temp, 1024, '\n');
	list_of_string_pointers[entry++] = temp;
}*/


filestr.read(buffer, length);
filestr.close();
char* start;
int entry = 0;
start = strtok(buffer,"\n");
while (start != 0)
	{
	list_of_string_pointers[entry++] = start;
	start = strtok (0,"\n");
	}
return(entry);
}



int HP_ReadTextFile::NumberOfEntries()
{
int number_of_lines = NumberOfLines();
int count = 0;
for(int x = 0; x < number_of_lines; x++)
  {
  if(array_of_strings->GetStringPointer(x)[0] == '>')
    {
    count++;
    }
  }
return(count);
}

int HP_ReadTextFile::NumberOfLines()
{
 return(array_of_strings->GetNumberOfStrings());
}

int HP_ReadTextFile::LengthOfLine(int index)
{
return(strlen(array_of_strings->GetStringPointer(index)));
}

HP_DynamicStringArray* HP_ReadTextFile::GetWholeEntry(HP_DynamicStringArray*& strings, int index)
{
  int number_of_lines = NumberOfLines();
  int start = GetLineForEntryNumber(index);
  int end = GetLineForEntryNumber(index+1);
  if (end < number_of_lines)
    end-=2;
  strings = new HP_DynamicStringArray();
  for(int x = start; x < end; x++)
    strings->AddString(array_of_strings->GetStringPointer(x));
return(strings);
}

char* HP_ReadTextFile::GetLine(char*& line, int index)
{
int linelength =  LengthOfLine(index);
line = new char[linelength+1];
memset(line,0,linelength+1);
strncpy(line, array_of_strings->GetStringPointer(index),linelength);
return(line);
}

char* HP_ReadTextFile::GetLinePointer(int index)
{
  return(array_of_strings->GetStringPointer(index));
}

int  HP_ReadTextFile::GetLineForEntryNumber(int index)
{
int line = 0;
int count = 0;
int number_of_lines = NumberOfLines();
while((line < number_of_lines) && (count <= index))
  {
  if(array_of_strings->GetStringPointer(line)[0] == '>')
    {
    count++;
    line++;
    }
  else
    {
    line++;
    }
  }
return(line);
}

char* HP_ReadTextFile::CombineStringsInEntry(char*& temp, int index)
{
  HP_DynamicStringArray* entry = 0;
  GetWholeEntry(entry, index);
  int number_of_strings = entry->GetNumberOfStrings();
  int length = 0;
  for(int x = 0; x < number_of_strings; x++)
    {
      length += strlen(entry->GetStringPointer(x));
    }
  temp = new char[length+1];
  memset(temp, 0, length+1);
  char* pointer = temp;
  for(int x = 0; x < number_of_strings; x++)
    {
      strcpy(pointer, entry->GetStringPointer(x));
      pointer += strlen(entry->GetStringPointer(x));
    }
return(temp);
}

int HP_ReadTextFile::ReadFileofFileNames(char* filename, HP_DynamicStringArray* dynamic_string_array)
{
char* temp = 0;
std::fstream inputfile;
inputfile.open(filename, std::fstream::in);
while (inputfile.eof() != true)
	{
	temp = new char[1024];
	memset(temp, 0, 1024);
	inputfile.getline(temp, 1024);
	dynamic_string_array->AddString(temp);
	}
inputfile.close();
int number_of_entries = dynamic_string_array->GetNumberOfStrings();
return(number_of_entries-1);
}

bool HP_ReadTextFile::IsTextFileofFloatValues()
{
if(strchr(GetLinePointer(0), '.'))
	return(true);
else
	return(false);
}

char* HP_ReadTextFile::StripFileExtension(char*& modified_string, char* full_path)
{
char* point = strchr(full_path, '.');
int full_string_length = strlen(full_path);
int extension_string_length = strlen(point);
modified_string = new char[1024];
memset(modified_string, 0, 1024);
strncpy(modified_string, full_path, full_string_length-extension_string_length);
return(modified_string);
}
  
