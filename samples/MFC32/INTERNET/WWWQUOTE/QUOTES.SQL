/* Database: Stocks */

/****** Object:  Table dbo.Issues ******/
if exists (select * from sysobjects where id = object_id('Issues') and sysstat & 0xf = 3)
	drop table Issues
GO

CREATE TABLE Issues
(
	CUSIP varchar (8) NULL ,
	Ticker varchar (7) NULL ,
	Name varchar (50) NULL
)
GO

INSERT INTO Issues (CUSIP, Ticker, Name)
VALUES ("09984310", "BORL", "BORLAND INTL INC/COM")
GO
INSERT INTO Issues (CUSIP, Ticker, Name)
VALUES ("59491810", "MSFT", "MICROSOFT CORP/COM")
GO
INSERT INTO Issues (CUSIP, Ticker, Name)
VALUES ("54570010", "LOTS", "LOTUS DEV CORP/COM")
GO
INSERT INTO Issues (CUSIP, Ticker, Name)
VALUES ("67000610", "NOVL", "NOVELL INC/COM")
GO


/****** Object:  Table dbo.Quotes ******/
if exists (select * from sysobjects where id = object_id('Quotes') and sysstat & 0xf = 3)
	drop table Quotes
GO								    

CREATE TABLE Quotes
(
	Ticker varchar (50) NULL ,
	Date datetime NULL ,
	Volume int NULL ,
	HighAsk varchar (50) NULL ,
	LowBid varchar (50) NULL ,
	CloseAvg varchar (50) NULL
)
GO

INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19941201  0:00",  1115500, "9 5/8", "8 5/8", "8 5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19941202  0:00", 1062600, "8 7/8", "8 1/4", "8 5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19941205  0:00", 298200, "8 5/8", "8 3/8", "8 7/16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19941206  0:00", 355600, "8 1/2", "8 1/4", "8 1/2")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19941207  0:00", 150300, "8 1/2", "8 1/4", "8 1/2")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19941208  0:00", 239100, "8 1/2", "8 1/4", "8 1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19941209  0:00", 584600, "9", "8 1/8", "8 3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19941212  0:00", 458900, "8 7/8", "8 1/4", "8 3/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19941213  0:00", 229000, "8 5/8", "8 1/4", "8 1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19941214  0:00", 207300, "8 3/8", "8 1/8", "8 1/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19941215  0:00", 187100, "8 3/8", "8 1/8", "8 1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19941216  0:00", 296000, "8 1/2", "8 1/8", "8 3/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19941219  0:00", 680200, "8 1/4", "7 1/4", "7 9/16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19941220  0:00", 794900, "7 5/8", "6 3/4", "7 1/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19941221  0:00", 527400, "8", "7 1/8", "7 1/2")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19941222  0:00", 1403300, "7 1/4", "6 1/2", "6 5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19941223  0:00", 713400, "6 3/4", "6 3/8", "6 1/2")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19941226  0:00", -1, NULL, NULL, NULL)
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19941227  0:00", 498000, "6 5/8", "6 1/4", "6 1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19941228  0:00", 708000, "6 1/2", "6", "6 1/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19941229  0:00", 895400, "6 1/4", "6", "6 1/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19941230  0:00", 1077900, "6 1/4", "6", "6 1/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950102  0:00", -1, NULL, NULL, NULL)
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950103  0:00", 713500, "7 1/4", "6 1/8", "7 1/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950104  0:00", 371900, "7", "6 1/2", "6 7/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950105  0:00", 254600, "6 7/8", "6 3/8", "6 5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950106  0:00", 171400, "6 7/8", "6 1/2", "6 7/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950109  0:00", 316000, "7", "6 5/8", "6 3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950110  0:00", 185000, "7 1/16", "6 5/8", "6 3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950111  0:00", 515000, "7 1/4", "6 3/4", "7")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950112  0:00", 962000, "7 1/2", "7", "7")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950113  0:00", 3093000, "8 1/8", "7", "7 3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950116  0:00", 1597500, "8 5/8", "8 1/8", "8 1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950117  0:00", 633900, "8 1/2", "8", "8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950118  0:00", 448500, "8 1/8", "7 3/4", "7 3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950119  0:00", 240600, "8", "7 3/4", "8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950120  0:00", 245600, "8", "7 5/8", "7 7/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950123  0:00", 485000, "7 7/8", "7 1/8", "7 7/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950124  0:00", 318600, "7 7/8", "7 1/4", "7 3/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950125  0:00", 657700, "7 1/4", "6 1/2", "7")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950126  0:00", 234600, "7 1/4", "7", "7")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950127  0:00", 406800, "7 1/2", "6 7/8", "6 7/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950130  0:00", 226500, "7", "6 5/8", "6 11/16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950131  0:00", 158400, "7 1/16", "6 5/8", "7")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950201  0:00", 116100, "7 1/4", "7", "7 1/16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950202  0:00", 255000, "7 3/8", "7", "7 3/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950203  0:00", 364400, "7 5/8", "7 1/4", "7 5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950206  0:00", 160600, "7 5/8", "7 3/8", "7 9/16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950207  0:00", 74600, "7 5/8", "7 3/8", "7 5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950208  0:00", 219900, "7 3/4", "7 7/16", "7 3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950209  0:00", 450600, "8", "7 1/2", "7 13/16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950210  0:00", 209700, "8", "7 3/4", "8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950213  0:00", 301500, "8", "7 3/4", "7 15/16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950214  0:00", 151600, "8", "7 3/4", "7 7/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950215  0:00", 294300, "8 1/8", "7 3/4", "7 7/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950216  0:00", 155900, "8", "7 5/8", "7 5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950217  0:00", 195700, "7 3/4", "7 1/2", "7 11/16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950220  0:00", -1, NULL, NULL, NULL)
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950221  0:00", 192000, "7 7/8", "7 5/8", "7 5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950222  0:00", 236000, "8", "7 5/8", "7 7/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950223  0:00", 549000, "8 1/8", "7 3/4", "8 1/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950224  0:00", 2403000, "9 1/2", "8", "8 3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950227  0:00", 1160600, "8 1/8", "7 5/8", "7 13/16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("BORL", "19950228  0:00", 372300, "8", "7 3/4", "8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19940121  0:00", 2694000, "63 13/32", "62  1/2", "62  9/16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19940122  0:00", 2357000, "63  1/4", "62", "63  1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19940125  0:00", 2385000, "64  1/4", "63  1/8", "63  5/16")
GO		     
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19940126  0:00", 2427000, "64  1/8", "63", "63  3/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19940127  0:00", 2297000, "63  7/8", "63", "63")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19940128  0:00", 2621000, "63  3/4", "62  1/4", "62  1/2")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19940129  0:00", 4997000, "63  1/4", "62  1/4", "63  1/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19941212  0:00", 4082000, "63  1/2", "61  5/8", "63  1/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19941213  0:00", 1889000, "63  1/2", "62  1/4", "62  3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19941214  0:00", 2103000, "63  5/8", "62  1/2", "63  1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19941215  0:00", 1803000, "64  1/8", "63", "63  1/2")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19941216  0:00", 3376000, "64  1/4", "63 13/64", "63  3/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19941219  0:00", 1969000, "63  5/8", "62  1/2", "62  5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19941220  0:00", 15846000, "62  3/4", "58  3/4", "59  7/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19941221  0:00", 4337000, "61  5/8", "60  1/8", "61  1/2")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19941222  0:00", 3585000, "62", "60  1/4", "60  5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19941223  0:00", 1218000, "61  3/8", "60  3/8", "60  7/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19941226  0:00", -1, NULL, NULL, NULL)
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19941227  0:00", 1285000, "61  5/8", "60  5/8", "60  3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19941228  0:00", 2185000, "61", "60", "61")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19941229  0:00", 1465000, "61  7/8", "60  5/8", "61 13/16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19941230  0:00", 1293000, "61  7/8", "61  1/8", "61  1/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950102  0:00", -1, NULL, NULL, NULL)
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950103  0:00", 2472000, "61  1/2", "60  1/8", "60  3/16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950104  0:00", 3226000, "60  3/4", "59  1/2", "60  5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950105  0:00", 2489000, "61", "59  3/8", "59  5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950106  0:00", 2918000, "61  1/4", "59  3/4", "60  5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950109  0:00", 2875000, "61", "59  3/4", "60  1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950110  0:00", 2637000, "61  1/2", "60", "61")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950111  0:00", 1970000, "61  3/4", "60  3/4", "61 19/64")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950112  0:00", 1687000, "62  1/8", "61", "61  1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950113  0:00", 2582000, "63", "61  7/8", "62  3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950116  0:00", 4666000, "64  1/2", "63", "64  1/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950117  0:00", 4621000, "65  1/8", "64  3/8", "64  5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950118  0:00", 3157000, "65  1/4", "64  1/4", "65  1/16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950119  0:00", 4194000, "65", "63  7/16", "63  1/2")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950120  0:00", 5150000, "63  1/2", "61  1/4", "61  5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950123  0:00", 3364000, "63", "60  1/2", "62 13/16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950124  0:00", 2296000, "63  1/8", "62", "62  1/64")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950125  0:00", 3279000, "62  1/4", "61", "61  3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950126  0:00", 5052000, "61", "59  3/4", "59  7/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950127  0:00", 3649000, "60  1/4", "59  3/4", "59  7/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950130  0:00", 3448000, "60  1/8", "58  7/8", "59")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950131  0:00", 3747000, "59  3/4", "58  1/4", "59  3/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950201  0:00", 4169000, "60  3/4", "58 63/64", "59  1/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950202  0:00", 2895000, "59  3/8", "58  3/8", "59")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950203  0:00", 3771000, "60  1/2", "59  1/4", "60  1/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950206  0:00", 3022000, "61  1/8", "60  1/8", "60  7/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950207  0:00", 2072000, "61  1/8", "60  5/8", "61")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950208  0:00", 3072000, "62", "61", "61  7/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950209  0:00", 2650000, "62  1/2", "61  3/4", "61  7/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950210  0:00", 3116000, "62  5/8", "61  1/2", "62")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950213  0:00", 2385000, "62  3/4", "61  5/8", "62")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950214  0:00", 1945000, "62  1/4", "61  1/2", "61  7/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950215  0:00", 9186000, "61  3/8", "59  5/8", "60  3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950216  0:00", 3547000, "61  1/8", "60  1/4", "60  7/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950217  0:00", 2364000, "60  7/8", "60  1/4", "60  3/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950220  0:00", -1, NULL, NULL, NULL)
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950221  0:00", 3462000, "60  3/4", "59  1/2", "59  3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950222  0:00", 3806000, "62", "59  5/8", "61  1/2")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950223  0:00", 3746000, "62  1/8", "60  1/2", "61  5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950224  0:00", 2136000, "61  7/8", "61  1/4", "61  1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950227  0:00", 1787000, "62", "61  1/8", "61  3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("MSFT", "19950228  0:00", 3284000, "63  1/4", "61  3/4", "63")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19941201  0:00", 3060000, "20", "19", "19  3/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19941202  0:00", 2909000, "19  3/4", "18  7/8", "19  1/2")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19941205  0:00", 874000, "19  5/8", "19  1/8", "19  1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19941206  0:00", 2058000, "19  1/4", "18  3/4", "19")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19941207  0:00", 1460000, "19  1/8", "18  5/8", "18  5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19941208  0:00", 2174000, "18  7/8", "18  3/8", "18  7/16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19941209  0:00", 3768000, "18  5/8", "17  5/8", "18")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19941212  0:00", 2374000, "18  1/8", "17  1/2", "17 11/16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19941213  0:00", 3144000, "17  3/4", "16  7/8", "17  1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19941214  0:00", 15446000, "17", "15  3/4", "15  7/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19941215  0:00", 4023000, "16  1/4", "15  3/4", "16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19941216  0:00", 4262000, "16  1/4", "15 27/32", "16  1/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19941219  0:00", 3500000, "16  3/8", "15  7/8", "16  1/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19941220  0:00", 3414000, "16  5/8", "16  1/8", "16  5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19941221  0:00", 3966000, "17  5/8", "16  3/4", "17  1/2")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19941222  0:00", 3706000, "18  1/8", "17  1/2", "17 15/16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19941223  0:00", 1298000, "18", "17  3/8", "17  1/2")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19941226  0:00", -1, NULL, NULL, NULL)
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19941227  0:00", 1099000, "17  3/4", "17  1/4", "17  5/16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19941228  0:00", 1646000, "17  1/2", "17", "17  1/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19941229  0:00", 2132000, "17  5/8", "17  1/8", "17  1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19941230  0:00", 2068000, "17  3/8", "16  3/4", "17  1/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950102  0:00", -1, NULL, NULL, NULL)
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950103  0:00", 2220000, "17  1/2", "16  7/8", "17  1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950104  0:00", 2419000, "17  1/2", "16  7/8", "17")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950105  0:00", 1910000, "17  1/4", "16  7/8", "17  1/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950106  0:00", 1720000, "17  3/8", "17", "17  3/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950109  0:00", 1432000, "17  5/8", "17  1/8", "17  3/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950110  0:00", 3523000, "18", "17  3/8", "17  5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950111  0:00", 2976000, "18", "17  5/8", "17 15/16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950112  0:00", 3202000, "18  3/8", "17  7/8", "18  5/16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950113  0:00", 9306000, "18  5/8", "17  7/8", "18  1/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950116  0:00", 3623000, "18  5/8", "17  7/8", "18  1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950117  0:00", 2540000, "18  3/8", "17  3/4", "18")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950118  0:00", 3140000, "18", "17  1/2", "17  7/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950119  0:00", 3087000, "18  1/8", "17  3/4", "18")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950120  0:00", 3685000, "18  3/8", "18", "18  1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950123  0:00", 2848000, "18  1/2", "17  3/4", "18  1/2")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950124  0:00", 2704000, "18  5/8", "18  1/8", "18  1/2")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950125  0:00", 3503000, "18  7/8", "18", "18  3/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950126  0:00", 2290000, "18  1/2", "17  7/8", "18")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950127  0:00", 1983000, "18  1/4", "17  7/8", "18")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950130  0:00", 3667000, "18  1/8", "17  5/8", "17 11/16")
GO	  
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950131  0:00", 2188000, "18", "17  1/2", "17  3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950201  0:00", 1932000, "18  1/8", "17  5/8", "18")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950202  0:00", 2302000, "18  1/4", "17  7/8", "18  1/16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950203  0:00", 2632000, "18  3/8", "17  7/8", "18")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950206  0:00", 2417000, "18  1/2", "18", "18  3/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950207  0:00", 6101000, "19  1/2", "18  3/8", "19  3/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950208  0:00", 9189000, "20  3/8", "20", "20  1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950209  0:00", 5485000, "20  1/4", "19 61/64", "20")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950210  0:00", 7452000, "20  7/8", "20", "20  7/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950213  0:00", 5114000, "21  1/2", "20  5/8", "21")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950214  0:00", 5016000, "21  1/8", "19  7/8", "20  1/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950215  0:00", 5184000, "21", "20", "20  1/2")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950216  0:00", 2861000, "20  1/2", "20", "20  1/16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950217  0:00", 2376000, "20  1/2", "19  7/8", "20")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950220  0:00", -1, NULL, NULL, NULL)
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950221  0:00", 1834000, "20  1/8", "19  3/4", "20")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950222  0:00", 4498000, "20  3/4", "19  7/8", "20  5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950223  0:00", 7067000, "21  3/8", "20  5/8", "20  5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950224  0:00", 3372000, "20  5/8", "19  7/8", "20  1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950227  0:00", 2819000, "20  1/4", "19  3/4", "20  1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("NOVL", "19950228  0:00", 9695000, "21", "20  1/4", "21")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19941201  0:00", 3318000, "44  3/4", "41  3/4", "44  1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19941202  0:00", 2270000, "45  3/4", "43  5/16", "45  1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19941205  0:00", 1980000, "46  1/4", "44  1/4", "44  3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19941206  0:00", 2416000, "44  3/4", "41", "41  3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19941207  0:00", 2350000, "43  1/4", "40  1/2", "42  1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19941208  0:00", 1478000, "41  3/4", "40  1/4", "40  3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19941209  0:00", 1812000, "40  3/4", "38  1/4", "40")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19941212  0:00", 874100, "41  1/2", "39  3/4", "41  1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19941213  0:00", 1085200, "42  1/2", "40", "40  3/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19941214  0:00", 848100, "41", "39  3/4", "40  3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19941215  0:00", 777200, "42", "40 31/64", "40  5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19941216  0:00", 534800, "41", "39  3/4", "39  3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19941219  0:00", 790000, "40  1/2", "39  1/2", "39  1/2")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19941220  0:00", 818000, "40", "38  3/4", "39  5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19941221  0:00", 514000, "40  3/4", "39  3/4", "40  5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19941222  0:00", 566000, "41  1/2", "40  1/2", "41")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19941223  0:00", 362800, "42", "40  3/4", "41  1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19941226  0:00", -1, NULL, NULL, NULL)
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19941227  0:00", 742000, "42  1/4", "39  5/8", "40")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19941228  0:00", 401000, "40", "39", "39  1/2")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19941229  0:00", 602000, "41  1/4", "39  5/8", "41  1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19941230  0:00", 937100, "42  3/16", "40  1/4", "41")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950102  0:00", -1, NULL, NULL, NULL)
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950103  0:00", 637000, "41  1/2", "39  1/2", "39  7/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950104  0:00", 432000, "40  1/4", "39  1/2", "40")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950105  0:00", 307000, "40  3/4", "39  3/4", "39  3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950106  0:00", 398000, "40  1/2", "39  1/2", "40")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950109  0:00", 2658000, "43", "40  3/4", "42")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950110  0:00", 1612000, "43", "41  7/8", "42  9/16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950111  0:00", 1177000, "44", "41  3/4", "43  7/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950112  0:00", 952000, "44  1/2", "43  1/2", "44  1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950113  0:00", 546000, "44  1/4", "42  1/2", "43")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950116  0:00", 639400, "42  3/4", "41  3/4", "42  1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950117  0:00", 852400, "43  1/4", "42", "43  1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950118  0:00", 376400, "43  1/4", "42  1/4", "43")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950119  0:00", 1166400, "43", "41", "41  3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950120  0:00", 636900, "41  3/4", "40  1/4", "40 15/16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950123  0:00", 811000, "41  1/2", "39  3/4", "41  1/2")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950124  0:00", 1203000, "41  1/2", "40  1/2", "40  5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950125  0:00", 5718000, "45  1/4", "38  1/2", "43  9/16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950126  0:00", 2012000, "45  3/4", "43  3/4", "44  3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950127  0:00", 871000, "45  1/4", "44", "44  3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950130  0:00", 666000, "45  3/4", "44  3/8", "44  3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950131  0:00", 1092000, "45  1/4", "44  1/8", "44  5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950201  0:00", 771000, "45  1/2", "44  3/4", "45  1/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950202  0:00", 1168000, "45  1/4", "43  1/2", "44")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950203  0:00", 1839000, "45  3/4", "44  1/4", "44  1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950206  0:00", 1312000, "47  1/4", "44  1/2", "47  1/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950207  0:00", 582000, "47  1/8", "45  3/4", "46 23/64")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950208  0:00", 608000, "47", "45  1/2", "45  3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950209  0:00", 729000, "47", "45  1/2", "46  1/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950210  0:00", 1022400, "47  1/2", "46", "47  1/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950213  0:00", 601000, "48  1/4", "46  3/4", "47  1/2")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950214  0:00", 612000, "48  1/4", "46  3/4", "46  3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950215  0:00", 879000, "48", "46", "46  3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950216  0:00", 407000, "47  1/4", "46  1/4", "46  3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950217  0:00", 498900, "46  1/2", "45  1/2", "45  1/2")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950220  0:00", -1, NULL, NULL, NULL)
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950221  0:00", 423700, "45  3/4", "44  3/4", "45  3/16")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950222  0:00", 472300, "45", "44", "44  3/4")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950223  0:00", 1259300, "44  3/4", "43  1/4", "43  1/2")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950224  0:00", 1066700, "43  1/2", "42  1/2", "42  1/2")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950227  0:00", 1144000, "43", "41  1/2", "41  5/8")
GO
INSERT INTO Quotes (Ticker, Date, Volume, HighAsk, LowBid, CloseAvg)
VALUES ("LOTS", "19950228  0:00", 1928000, "42", "40", "41  3/4")
GO

SELECT "=== FINISHED! ==="
GO


