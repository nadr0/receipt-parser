/* 
	char * fileName - is the the txt file name
					- i.g. "myFileNameGoesHere.txt"
	char * number - is just to number the csv files 
*/
void parseTXTtoCSV(char * fileName, char * number);


/* 
	Pass in line from getline
	checks to see if it needs to grab more lines
	to find the end of the item

	returns 1 if end of the item
	returns 0 if NOT end of the item

*/
int endOfItem(char * item);


/* 
	Checks to stop reading .txt
*/
int stopReading(char * line);


/* 
	Returns a string containing the item price
	user needs to free this returned string
*/
char * retrieveItemPrice(char * line);


/* 
	Returns a string containing the item name
	user needs to free this returned string
*/
char * retrieveItemName(char * line);


/* 
	Returns 1 if you should skip the line 
	Returns 0 if item is still being processed
*/
int skipLineAfterItemProcessed(char * line);


/* Gets the itemName if there is no ending char */
char * retrieveItemNameNoEnd(char * line);

/* 	
	Get the item quantity if any 
	Need to free this memory that is returned
*/
char * retrieveItemQuantity(char * line);


/* 
	Get the mPerks Offer
*/
char * retrieve_mPerksOffer(char * line);

/* 
	Writes the name, price, and quantity into the csv file
*/
void addToCSV(char * name, char * price, char * quantity, FILE * csv);

/* 
	Writes the mPerks into the csv file
*/
void addMPerksToCSV(char * mPerks, FILE * csv);