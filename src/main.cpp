/********************************************************************************/
/*                                                                              */
/*    Copyright 2012 Alexander Vorobyev (Voral)                                 */
/*    http://va-soft.ru/                                                        */
/*                                                                              */
/*    This file is part of splitter project                                     */
/*                                                                              */
/*    Basetest is free software: you can redistribute it and/or modify          */
/*    it under the terms of the GNU General Public License as published by      */
/*    the Free Software Foundation, either version 3 of the License, or         */
/*    (at your option) any later version.                                       */
/*                                                                              */
/*    Basetest is distributed in the hope that it will be useful,               */
/*    but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*    GNU General Public License for more details.                              */
/*                                                                              */
/*    You should have received a copy of the GNU General Public License         */
/*    along with splitter.  If not, see <http://www.gnu.org/licenses/>.         */
/*                                                                              */
/********************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define ECODE_OK 0
#define ECODE_FILE_IN 1
#define ECODE_FILE_OUT 2
#define ECODE_NO_PARAM 3

void makeMask(char *name, char *mask);
unsigned int getRows(char *arg);

int main(int argc, char** argv)
{
	unsigned int rows = 10000;
	if (argc < 2)
	{
		printf("splitter v.1.0 (c) 2012 Alexander Vorobyev, http://va-soft.ru/\n\nUsing: splitter [options] <input_filename>\n"
				"\tOptions:\n"
				"\t\t-s\tShow the progress information\n"
				"\t\t-rX\tX rows per file (by default 10000 rows)\n"
				"\tSample:\n"
				"\t\tsplitter -s -t2000 input.txt\n"
		);
		return ECODE_NO_PARAM;
	}
	int c, index, needShow = 0;
	for (c = 1; c< argc; ++c)
	{
		if (argv[c][0]!='-') index = c;
		else if (argv[c][1]=='s') needShow = 1;
		else if (argv[c][1]=='r') rows = getRows(argv[c]);
	}
	
	int exitCode = ECODE_OK;
	char *mask = (char*)malloc((sizeof(argv[index])+5)*sizeof(char));
	char *fileName = (char*)malloc((sizeof(argv[index])+10)*sizeof(char));
	makeMask(argv[index],mask);
	
	FILE * fin, *fout;
	int fileNumber = 0;
	int n = 0;
	fin=fopen (argv[index],"r");
	if (fin==NULL) exitCode = ECODE_FILE_IN;
	else
	{
		sprintf(fileName,mask,fileNumber++);
		fout=fopen (fileName,"w");
		if (fout==NULL) exitCode = ECODE_FILE_OUT;
		else
		{
			if (needShow) printf("\r%d",n+1);
			do {
				c = fgetc (fin);
				if (c == '\n')
				{
					n++;
					if (needShow) printf("\r%d",n+1);
					if (n==rows)
					{
						fclose (fout);
						sprintf(fileName,mask,fileNumber++);
						fout=fopen (fileName,"w");
						if (fout==NULL) 
						{
							exitCode = ECODE_FILE_OUT;
							break;
						}
						n=0;
					}
				}
				fputc(c,fout);
			} while (c != EOF);
			fclose (fout);
		}
		fclose (fin);
	}
	printf("\r");
	if (exitCode==ECODE_FILE_IN) perror ("Error opening input file\n");
	else if (exitCode==ECODE_FILE_OUT) perror ("Error opening output file\n");
	
	free(mask);
	free(fileName);
	
	return exitCode;
}
void makeMask(char *name, char *mask)
{
	int fAdded = 0;
	for(;*name != '\0';++name)
	{
		if (*name == '.')
		{
			fAdded = 1;
			*mask = '_';
			++mask;
			*mask = '%';
			++mask;
			*mask = '0';
			++mask;
			*mask = '3';
			++mask;
			*mask = 'd';
			++mask;
		}
		*mask = *name;
		++mask;
	}
	if (fAdded == 0)
	{
		*mask = '_';
		++mask;
		*mask = '%';
		++mask;
		*mask = '0';
		++mask;
		*mask = '3';
		++mask;
		*mask = 'd';
	}
}
unsigned int getRows(char *arg)
{
	unsigned int result = 0;
	arg = arg + 2;
	while (*arg!='\0')
	{
		result =  result * 10 + (*arg) - '0';
		++arg;
	}
	return result;
}
