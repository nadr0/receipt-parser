#define _GNU_SOURCE
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


#include "parsePDF.h"


void parseTXTtoCSV(char * fileName, char * number){

	/* Open .txt file*/
	FILE * f = fopen(fileName,"r");
	if(!f){
		fprintf(stderr, "%s failed to open\n", fileName);
		return;
	}

	/* Used for getline */
	size_t len = 0;
	char * line = NULL;

	printf("PROCESSING : %s\n", fileName);

	while( (getline(&line, &len, f)) != -1){
		/* Check to see if we find GROCERY */
		if(strcmp(line,"GROCERY\n") == 0){
			break;
		}
	}

	/*
		Set to 1 if you just read a new item 
		Set to 0 if you just read the item name of the new item
		Gets set back to 1 if read N, FT, T
	*/

	FILE * myFile = NULL;
	char * myFileName = calloc(1,strlen(number) + strlen("meijer.csv") + 1);
	strcat(myFileName,"meijer");
	strcat(myFileName, number);
	strcat(myFileName,".csv");
	myFile = fopen(myFileName, "w+");

	int itemNameFound = 0;
	int itemPriceFound = 0;

	/* If item seen is 1, then just saw an item 
		Used for skipping lines after an item is processed if another item is not next */
	int itemSeen = 0;

	char * itemPrice = "3.00";
	char * itemName = NULL;
	char * itemQuantity = NULL;
	char * mPerks = NULL;

	char string_mPerksTotal[100];
	float mPerksTotal = 0.0;

	char * tempItemQuantity = NULL;

	char * end_mPerks = NULL;

	fwrite("Item,Price,Quantity,mPerks",sizeof(char),strlen("Item,Price,Quantity,mPerks"),myFile);


	/*
		PARSING THE TXT FILE INTO
			NAME,PRICE,QUANTITY,mPERKS
	*/
	while( (getline(&line, &len, f)) != -1 ){
		/* WE have reached our EOF */
		if(stopReading(line)){
			break;
		}

		/* If we just saw an item and the next line contains a @ symbol, skip the line */
		if(itemSeen == 1 && skipLineAfterItemProcessed(line)){
			mPerks = retrieve_mPerksOffer(line);
			end_mPerks = retrieve_mPerksOfferAtTheEnd(line);
			if(end_mPerks){
				add_end_MPerksToCSV(end_mPerks, myFile);
				end_mPerks = NULL;
			}
			if(mPerks){
				mPerksTotal += atof(mPerks);	
			}

		}else{

			itemSeen = 0;

			if(!itemNameFound){
				if(mPerksTotal != 0.0){
					memset(string_mPerksTotal,0,100);
					sprintf(string_mPerksTotal,"%f",mPerksTotal);
					addMPerksToCSV(string_mPerksTotal, myFile);
					mPerks = NULL;
				}else{
					addMPerksToCSV("\0", myFile);
				}
				mPerksTotal = 0.0;
				/* Check if it has ending char or not */
				if(endOfItem(line)){
					/* Has ending Char */
					itemName = retrieveItemName(line);
				}else{
					/* Doesn't have ending char */
					itemName = retrieveItemNameEnd(line);
				}
				itemNameFound = 1;
			}

			tempItemQuantity = retrieveItemQuantity(line);
			if(tempItemQuantity != NULL){
				itemQuantity = tempItemQuantity;
			}	

			if(!itemPriceFound && endOfItem(line)){
				itemPrice = retrieveItemPrice(line);
				itemPriceFound = 1;
			}

			if(itemPriceFound && itemNameFound){
				itemSeen = 1;
				addToCSV(itemName, itemPrice, itemQuantity, myFile);

				/* Frees */
				free(itemName);
				free(itemPrice);
	
				if(itemQuantity){
					free(itemQuantity);
					itemQuantity = NULL;
				}

				itemName = NULL;
				itemPrice = NULL;

				/* Reset itemQuantity every time*/
				itemQuantity = NULL;
				itemPriceFound = 0;
				itemNameFound = 0;
			}
		}

	}

	free(myFileName);
	free(line);
	fclose(f);
	fclose(myFile);

}

char * retrieveItemName(char * line){
	char * copyLine = calloc(1, strlen(line)+1);
	strcpy(copyLine, line);

	char * splitString = strtok(copyLine," ");
	int ptrMath = strlen(splitString)+1;

	char * ptr = copyLine + ptrMath;

	int itemNameLength = 0;

	while( ((int)*ptr < 47 || (int)(*ptr) > 57) && (int)(*ptr) != 46){
		itemNameLength++;
		ptr++;
	}

	char * itemName = calloc(1, itemNameLength + 1);
	ptr = ptr - itemNameLength;

	while( ((int)*ptr < 47 || (int)(*ptr) > 57) && (int)(*ptr) != 46){
		*itemName = *ptr;
		ptr++;
		itemName ++;
	}

	itemName -= itemNameLength;

	/* EXTRA BYTE (32) SPACE CHAR*/
	if(itemName[itemNameLength-1] == ' '){
		itemName[itemNameLength-1] = '\0';
	}

	/* Free the line */
	free(copyLine);	

	return itemName;

}	


char * retrieveItemNameEnd(char * line){

	char * copyLine = calloc(1, strlen(line)+1);
	strcpy(copyLine, line);

	char * splitString = strtok(copyLine," ");
	int ptrMath = strlen(splitString)+1;

	char * ptr = copyLine + ptrMath;

	int ptrLength = strlen(ptr);

	/* Remove \n char at the end of the string? */
	ptr[ptrLength-1] = '\0';

	char * itemName = calloc(1, strlen(ptr) + 1);
	strcpy(itemName, ptr);

	free(copyLine);

	return itemName;
}	

char * retrieveItemPrice(char * line){	

	size_t lineLength = strlen(line);
	/* Goto end of the string at the ending char N, FT, T */
	char * ptr = line + lineLength;
	/* Goto first space char, going backwards */
	while(*ptr != ' '){
		ptr--;
	}
	/* Move to the last digit*/
	ptr--;

	int priceStringLength = 0;

	while(*ptr != ' '){
		priceStringLength++;
		ptr--;
	}

	char * priceString = calloc(1,priceStringLength+1);
	/* Move forward to first digit */
	ptr++;

	while(*ptr != ' '){
		*priceString = *ptr;
		ptr++;
		priceString++;
	}
	return priceString - priceStringLength;
}

int stopReading(char * line){
	char * searchedLine = strstr(line,"Mperks #");
	if(searchedLine){
		return 1;
	}
	return 0;
}

int endOfItem(char * item){

	char * searchedString_N = strstr(item, " N\n");
	char * searchedString_FT = strstr(item, " FT\n");
	char * searchedString_T = strstr(item, " T\n");

	if(searchedString_N || searchedString_FT || searchedString_T){
		return 1;
	}

	return 0;
}

int skipLineAfterItemProcessed(char * line){

	char * searchString_for_AT = strstr(line, "@");
	char * searchString_for_EQUALS_ARROW = strstr(line,"=>");
	char * searchString_for_mPerks_Offer = strstr(line,"mPerks Offer");

	if(searchString_for_AT || searchString_for_EQUALS_ARROW || searchString_for_mPerks_Offer){
		return 1;
	}

	return 0;
}

char * retrieve_mPerksOffer(char * line){

	char * searchString_for_EQUALS_ARROW = strstr(line,"=>");

	/* If there is no end char */
	// !endOfItem(line) && 
	if(searchString_for_EQUALS_ARROW){
		int stringMPerksLength = 0;
		char * ptr = line;
		/* = > _  (equals, arrow, space) move the ptr forward*/
		ptr += 3;
		while(*ptr != ' '){
			stringMPerksLength++;
			ptr++;
		}
		ptr = ptr - stringMPerksLength;
		char * stringMPerk = calloc(1, stringMPerksLength + 1);
		while(*ptr != ' ')
		{
			*stringMPerk = *ptr;
			stringMPerk++;
			ptr++;
		}
		stringMPerk = stringMPerk - stringMPerksLength;
		return stringMPerk;
	}

	return NULL;
}

char * retrieve_mPerksOfferAtTheEnd(char * line){

	char * searchString_for_EQUALS_ARROW = strstr(line,"=>");

	/* If there is no end char */
	if(!endOfItem(line) && searchString_for_EQUALS_ARROW){
		int stringMPerksLength = 0;
		char * ptr = line;
		/* = > _  (equals, arrow, space) move the ptr forward*/
		ptr += 3;
		while(*ptr != ' '){
			stringMPerksLength++;
			ptr++;
		}
		ptr = ptr - stringMPerksLength;
		char * stringMPerk = calloc(1, stringMPerksLength + 1);
		while(*ptr != ' ')
		{
			*stringMPerk = *ptr;
			stringMPerk++;
			ptr++;
		}
		stringMPerk = stringMPerk - stringMPerksLength;
		return stringMPerk;
	}

	return NULL;
}


char * retrieveItemQuantity(char * line){

	char * searchString_for_AT = strstr(line,"@");

	if(searchString_for_AT){
		int quantityStringLength = 0;
		char * ptr = line;
		/* Deteremine the number of chars the quantity requires*/
		while(*ptr != ' '){
			quantityStringLength++;
			ptr++;
		}
		char * quantityString = calloc(1,quantityStringLength + 1);
		ptr = ptr - quantityStringLength;
		/* Copy the quantity */
		while(*ptr != ' '){
			*quantityString = *ptr;
			quantityString++;
			ptr++;
		}

		/* Reset the position */
		quantityString = quantityString - quantityStringLength;

		return quantityString;
	}

	return NULL;
}

void addToCSV(char * name, char * price, char * quantity, FILE * csv){
	char * oneItem = "1";

	fwrite(name, strlen(name)+1, sizeof(char), csv);
	fwrite(",",1,1,csv);
	fwrite(price, strlen(price)+1, sizeof(char), csv);
	fwrite(",",1,1,csv);
	if(quantity){
		fwrite(quantity, strlen(quantity)+1, sizeof(char), csv);
	}else{
		fwrite(oneItem, strlen(oneItem)+1, sizeof(char), csv);
	}	
	fwrite(",",1,1,csv);
	// fwrite("\n",1,1,csv);

}

void addMPerksToCSV(char * mPerks, FILE * csv){
	// fwrite(",",1,1,csv);
	// fwrite(",",1,1,csv);
	// fwrite(",",1,1,csv);
	fwrite(mPerks, strlen(mPerks)+1, sizeof(char), csv);
	fwrite("\n",1,1,csv);
}


void add_end_MPerksToCSV(char * mPerks, FILE * csv){
	fwrite("\n",1,1,csv);
	fwrite(",",1,1,csv);
	fwrite(",",1,1,csv);
	fwrite(",",1,1,csv);
	fwrite(mPerks, strlen(mPerks)+1, sizeof(char), csv);
	fwrite("\n",1,1,csv);
}

