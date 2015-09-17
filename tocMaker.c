#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define true 1
#define false 0

//Declare Functions
int isHeadline(char* line);
char* getName(char* headline);
char* getLink(char* name);
char* createTopic(char* line);
void trim(char* str);
void replace(char* str, char oldChar, char newChar);
void removeNeedle(char* str, char needle);
void removeNeedles(char* str, char* needle);

void addLink(char* str);
char hasItem(char* str);
void cleanupLinks();
void toLower(char* str);

void appendBuf(char* str);
void cleanupBuffer();
int readLine(FILE *file, char* buffer, int maxSize);

//declare variables for a link-list
char** links;
int linksize;
int linksEnd;

//declare buffer for output file
char** outputBuffer;
int buffSize;
int buffEnd;

int main(int argc, char** argv) {

	links = malloc(sizeof(char*));
	linksize = 1;
	linksEnd = 0;

	outputBuffer = malloc(sizeof(char*));
	buffSize = 1;
	buffEnd = 0;

	char cwd[1024];
	char* filePath = NULL;
	if (getcwd(cwd, sizeof(cwd)) != NULL && argc > 1) {
		//check if absolut or relative path of file is given
		if (argv[1][0] != '/') {
			filePath = malloc(strlen(cwd) + strlen(argv[1]) + 2);
			sprintf(filePath, "%s/%s%c", cwd, argv[1], '\0');
		} else {
			filePath = malloc(strlen(argv[1] + 1));
			strcpy(filePath, argv[1]);
		}
	}

	//check if filepath is ok
	if (filePath != NULL) {

		FILE *file;

		file = fopen(filePath, "r");
		if (file) {

			//read file
			char lineBuffer[1024];
			while (readLine(file, lineBuffer, 1024)) {
				appendBuf(lineBuffer);
			}

			//cwrite file and create table of content
			file = freopen(filePath, "w", file);

			//skip initial headline
			int i = 0;
			while (i < buffEnd && (strlen(outputBuffer[i]) < 2
					|| outputBuffer[i][0] != '#' || outputBuffer[i][1] != '#')) {
				fprintf(file, "%s\n", outputBuffer[i]);
				i++;
			}

			//crate toc
			fprintf(file, "## Table of Content\n\n");
			for (int index = i; index < buffEnd; index++) {
				char* topic = createTopic(outputBuffer[index]);
				if (topic) {
					fprintf(file, "%s\n", topic);
					free(topic);
				}
			}
			fprintf(file, "\n");

			//write the rest to the file
			while (i < buffEnd) {
				fprintf(file, "%s\n", outputBuffer[i]);
				i++;
			}

			fclose(file);
		} else {
			printf("Error: could not open file\n");
		}
	} else {
		printf("Error: %s\n", argc < 2?"need a README file":"filepath is to lang");
	}

	if (filePath)
		free(filePath);

	cleanupLinks();
	return 0;
}

int readLine(FILE *file, char* buffer, int maxSize) {
	int i = 0;
	char curChar;
	while ((curChar = fgetc(file)) != EOF && curChar != '\n' && i < (maxSize - 1)) {
		buffer[i] = curChar;
		i++;
	}
	buffer[i] = '\0';

	return curChar != EOF || i > 0;
}

int isHeadline(char* line) {
	int hashtags = 0;

	hashtags = strspn(line, "#");
	return hashtags;
}

char* getName(char* headline) {
	char* name = malloc(strlen(headline));
	strcpy(name, headline);
	removeNeedle(name, '#');
	trim(name);
	return name;
}

char* getLink(char* name) {
	char *tmpName = getName(name);
	trim(tmpName);
	removeNeedles(tmpName, "()[]{}");
	replace(tmpName, ' ', '-');

  //allocate space (+2 for #  and \0, +3 for possible extensions like _1)
	char* link = malloc(strlen(tmpName) + 5);
	link[0] = '#';
	strcpy(link + 1, tmpName);
	free(tmpName);
	toLower(link);

	//check if link-name is allready in use and modify if necessary
	int linksEnd = strlen(link);
	int counter = 1;
	char buf[5];
	while (hasItem(link)) {
		sprintf(buf, "_%d%c", counter++, '\0');
		strcpy(link + linksEnd, buf);
	}

	addLink(link);
	return link;
}

char* createTopic(char* line) {
	int numHashtag = isHeadline(line);
	if (numHashtag) {
		int numSpaces = 2 * numHashtag -1;
		char *name = getName(line);
		char *link = getLink(name);
		char *topic = malloc(strlen(name) + strlen(link) + numSpaces + 8);

		for(int i = 0; i < numSpaces; i++)
			topic[i] = ' ';

		sprintf(topic + numSpaces, "* [%s](%s)%c", name, link, '\0');

		free(name);
		return topic;
	} else
	return NULL;
}

void trim(char* str) {
	//trim front
	int linksEnd = strlen(str);

	if (strchr(str, ' ') == str) {
		int numSpaces = strspn(str, " ");
		for (int i = numSpaces; i <= linksEnd; i++)
			str[i - numSpaces] = str[i];
	}

	linksEnd = strlen(str);

	//trim back
	while (linksEnd > 0 && str[linksEnd - 1] == ' ') {
		str[linksEnd - 1] = str[linksEnd];
		linksEnd--;
	}
}

void replace(char* str, char oldChar, char newChar) {
	char* ptr = strchr(str, oldChar);
	while (ptr) {
		*ptr = newChar;
		ptr = strchr(str, oldChar);
	}
}

void removeNeedle(char* str, char needle) {
	char *ptr = strchr(str, needle);
	while (ptr) {
		for (int i = 0; i < strlen(ptr); i++)
			ptr[i] = ptr[i + 1];

		ptr = strchr(str, needle);
	}
}

void removeNeedles(char* str, char* needles) {
	for (int i = 0; i < strlen(needles); i++)
		removeNeedle(str, needles[i]);
}

void addLink(char* str) {
	//enlarge array if necessary
	if (linksEnd == linksize) {
		char** newBuffer = realloc(links, 2 * sizeof(char*) * linksize);
		linksize *= 2;
		links = newBuffer;
	}
	//add item to buffer
	links[linksEnd++] = str;
}

char hasItem(char* str) {
	for (int i = 0; i < linksEnd; i++)
		if (!strcmp(links[i], str))
			return true;

	return false;
}

void toLower(char* str) {
	for (;*str ; str++)
		if (*str > 64 && *str < 91)
			*str += 32;
			//should implement umlauts here
}

void cleanupLinks() {
	for (int i = 0; i < linksEnd; i++)
		free(links[i]);
	free(links);
}

void appendBuf(char* str) {
	//enlarge array if necessary
	if (buffEnd == buffSize) {
		char** newBuffer = realloc(outputBuffer, 2 * sizeof(char*) * buffSize);
		buffSize *= 2;
		outputBuffer = newBuffer;
	}
	//add item to buffer
	char* strCpy = malloc(strlen(str) + 1);
	strcpy(strCpy, str);
	outputBuffer[buffEnd++] = strCpy;
}

void cleanupBuffer() {
	for (int i = 0; i < buffEnd; i++)
		free(outputBuffer[i]);
	free(outputBuffer);
}