// RadioFilter.cpp : Defines the entry point for the console application.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

char *FindGoodPart(char *compPart);

int main(int argc, char *argv[])
{
	printf("main ==>\n");
	char currData[BUFSIZ];
	char prevData[BUFSIZ];
	size_t index = 0;

	prevData[0] = '\0';

	// forever
	while (1)
	{
		// Get the next character from stdin
		char buffer[1];
		read(0, buffer, sizeof(buffer));
//		printf("%c", buffer[0]);

		// If there was no more data, we be done
		if (buffer[0] == EOF)
			break;

		// If the characer is not a carriage return or line feed
		if (buffer[0]!= '\n' && buffer[0] != '\r')
		{
			// If there is room in the array, add the character.
			if (index < BUFSIZ)
			{
				currData[index++] = buffer[0];
				currData[index] = '\0';
			}
		}

		// We got the carriage return or line feed
		else
		{
			// Do we care about that record?
			char *goodPart = FindGoodPart(currData);
			if (goodPart)
			{
//				printf("============================\n");
//				printf("Good part: [%s]\n", goodPart);
//				printf("Prev data: [%s]\n", prevData);
//				printf("============================\n\n");

				// We do, because it is not a 1179998
				// address, but is a 11xxxxx address.
				// Is it the same as the last data printed?
				if (strcmp(goodPart, prevData) != 0)
				{
					// No, it isn't, so show it
					printf("%s\n", goodPart);
					printf("\n");

					// Now, make a copy of what was just printed
					strcpy(prevData, goodPart);
				}
			}

			// Reset to read the next record
			index = 0;
			currData[index] = '\0';
		}
	}
	printf("main <==\n");
	return 0;
}

char *FindGoodPart(char *record)
{
	char *goodPart = NULL;
	if (record)
	{
//		printf("Record: [%s]\n", record);

		char *address = strstr(record, "Address");
		if (address)
		{
			address += 9;
			char *interestingData = strdup(address);
			if (interestingData)
			{
//				printf("Interesting data: [%s]\n", interestingData);
				char *token = strtok(interestingData, " ");
//				printf("token [%s]\n", token);
				if (strcmp(token, "1179998") != 0 &&
					strcmp(token, "0") != 0)
				{
					if (token[0] == '1' && token[1] == '1')
					{
						char *oTri = strchr(address, '<');
						char *cTri = strchr(address, '>');
						if (oTri && cTri)
						{
							oTri[0] = '\0';
						}

						char *alpha = strstr(address, "Alpha: ");
						if (alpha)
						{
							goodPart = alpha + 9;
//							printf("Good part: [%s]\n", goodPart);
						}
					}
				}
				free(interestingData);
			}
		}
	}
	return goodPart;
}
