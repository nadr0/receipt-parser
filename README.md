# receipt-parser


I wrote a meijer receipt parser that takes pdf receipts from mPerks then returns a csv for each pdf. 

The csv files only contain GROCERY items. 

---

### Using the program

```
make
./m
```

The program will search your current directory for all .pdf files and feed it into TET, which takes the pdf and turns it into a raw text file.

The text file of the pdf will then be parsed into a csv file, the template is below. 

mPerks is the mPerks discount for an item. The date is the date on the receipt and is always in the bottom right hand cell of the CSV. 

Product Name  | Price Per Unit | Quantity  | mPerks
------------- | ------------- | ------------- | -------------
|MILK		|1.89	|1	|
|CHOBANI 4PK	|3.79	|2	|	
|ORANGE JUICE	|3.99	|2	|	
|PROTEIN BAR	|2.79	|1	|	
|CEREAL		|2.79	|1	|
|CLOROX FOAMER	|2.89	|1	|1	
|BATH CLEANER	|2.99	|1	|1	
|BATH CLEANER	|2.99	|2	|2	
|CLEMENTINES	|3.69	|1	|	
|FACIAL TISSUE	|5.49	|1	|1	
|ICE CREAM	|3	|1	|	
|ICE CREAM	|3	|2	|	
|ICE CREAM BARS	|3	|2	|	
|TRASH BAGS	|7.49	|1	|1	
|		|	|	|3/8/16

```
http://www.pdflib.com/pdflib-gmbh/ (PDFlib TET, does the pdf to text conversion)
```
