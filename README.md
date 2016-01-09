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


Product Name  | Product Price | Quantity  | mPerks
------------- | ------------- | ------------- | -------------
CUCUMBERS	|2.76	|4	
CILANTRO|	0.89|	1	
ORG CARROTS	|1.79|	1	
CHICKEN THIGHS|	4.79|	1	
CANNED PEAS|	0.57	|1	
CANNED VEGGIES	|2.22|	1	
TONIC WATER	|2.37 |	1	
CANNED PEAS	|1.29|	1	
GRND TURKEY	|10.74|	1	
SHREDDED CHEESE|	2|	1	
MILK ALTERNATVE|	3	|1	
SHREDDED CHEESE|	2	|1	
SHREDDED CHEESE	|2	|1	


```
http://www.pdflib.com/pdflib-gmbh/ (PDFlib TET, does the pdf to text conversion)
```
