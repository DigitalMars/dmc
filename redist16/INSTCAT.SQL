/*
**	INSTCAT.SQL
**	Installs catalog stored procedures on the Microsoft SQL Server.
**	Copyright 1994, Microsoft Corp.  All rights reserved.
*/

/*
NOTE:  you MUST change the last row inserted into spt_server_info
to be version number of this file.	the convention is jj.nn.dddd, where
jj is the major version number ("01" now), nn is the minor version number
("01" now), and dddd is the date in the form of month and day (mmdd) of
the date you check in this file.  Add (year-1991)*12 to the month to keep
in sync with the driver version numbers, i.e. checking in on Feb 5 1993
would mean setting the value to 01.01.2605.
*/

/****************************************************************************/
/* This portion sets up the ability to perform all the functions in this    */
/* script                                                                    */
/****************************************************************************/
use master
go
dump tran master with truncate_only
go

if exists (select * from sysobjects
	   where name = 'sp_configure' and sysstat & 7 = 4)
begin
    execute sp_configure 'update',1
end
reconfigure with override
go

/*
** If old versions of tables exist, drop them.
*/
if (exists (select * from sysobjects
		where name = 'MSdatatype_info' and sysstat & 7 = 3))
    drop table MSdatatype_info
go
if (exists (select * from sysobjects
		where name = 'MSdatatype_info_ext' and sysstat & 7 = 3))
    drop table MSdatatype_info_ext
go
if (exists (select * from sysobjects
		where name = 'MStable_types' and sysstat & 7 = 3))
    drop table MStable_types
go
if (exists (select * from sysobjects
		where name = 'MSserver_info' and sysstat & 7 = 3))
    drop table MSserver_info
go
if (exists (select * from sysobjects
		where name = 'spt_table_types' and sysstat & 7 = 3))
	drop table spt_table_types	/* no longer used */
go

/*
** If tables or procs already exist, drop them.
*/

if (exists (select * from sysobjects
		where name = 'spt_datatype_info' and sysstat & 7 = 3))
    drop table spt_datatype_info
go
if (exists (select * from sysobjects
		where name = 'spt_datatype_info_ext' and sysstat & 7 = 3))
    drop table spt_datatype_info_ext
go
if (exists (select * from sysobjects
		where name = 'spt_server_info' and sysstat & 7 = 3))
    drop table spt_server_info
go
if (exists (select * from sysobjects
		where name = 'sp_tables' and sysstat & 7 = 4))
    drop proc sp_tables
go
if (exists (select * from sysobjects
		where name = 'sp_statistics' and sysstat & 7 = 4))
    drop proc sp_statistics
go
if (exists (select * from sysobjects
		where name = 'sp_columns' and sysstat & 7 = 4))
    drop proc sp_columns
go
if (exists (select * from sysobjects
		where name = 'sp_fkeys' and sysstat & 7 = 4))
    drop proc sp_fkeys
go
if (exists (select * from sysobjects
		where name = 'sp_pkeys' and sysstat & 7 = 4))
    drop proc sp_pkeys
dump tran master with truncate_only
go

go
if (exists (select * from sysobjects
		where name = 'sp_stored_procedures' and sysstat & 7 = 4))
    drop proc sp_stored_procedures
go
if (exists (select * from sysobjects
		where name = 'sp_sproc_columns' and sysstat & 7 = 4))
    drop proc sp_sproc_columns
go
if (exists (select * from sysobjects
		where name = 'sp_table_privileges' and sysstat & 7 = 4))
    drop proc sp_table_privileges
go
if (exists (select * from sysobjects
		where name = 'sp_column_privileges' and sysstat & 7 = 4))
	drop proc sp_column_privileges
go
if (exists (select * from sysobjects
		where name = 'sp_server_info' and sysstat & 7 = 4))
    drop proc sp_server_info
go
if (exists (select * from sysobjects
		where name = 'sp_datatype_info' and sysstat & 7 = 4))
    drop proc sp_datatype_info
go
if (exists (select * from sysobjects
		where name = 'sp_special_columns' and sysstat & 7 = 4))
    drop proc sp_special_columns
go
if (exists (select * from sysobjects
		where name = 'sp_databases' and sysstat & 7 = 4))
	drop proc sp_databases
go

dump tran master with truncate_only
go

print "creating table spt_datatype_info_ext"
go
create table spt_datatype_info_ext (
				user_type		smallint	not null,
				create_params	varchar(32) null)
go

grant select on spt_datatype_info_ext to public
go


insert into spt_datatype_info_ext
	/* CHAR 	 user_type, create_params */
	values			 (1,	"length" )

insert into spt_datatype_info_ext
	/* VARCHAR	 user_type, create_params */
	values			 (2,	"max length" )

insert into spt_datatype_info_ext
	/* BINARY	 user_type, create_params */
	values			 (3,	"length" )

insert into spt_datatype_info_ext
	/* VARBINARY user_type, create_params */
	values			 (4,	"max length" )

insert into spt_datatype_info_ext
	/* SYSNAME	 user_type, create_params */
	values			 (18,	"max length" )
go

print "creating table spt_datatype_info"
go
create table spt_datatype_info (
				ss_dtype		   tinyint		not null,
			    type_name          varchar(32)  not null,
			    data_type          smallint     not null,
			    data_precision     int          null,
			    numeric_scale      smallint     null,
			    numeric_radix      smallint     null,
			    length             int          null,
			    literal_prefix     varchar(32)  null,
			    literal_suffix     varchar(32)  null,
			    create_params      varchar(32)  null,
			    nullable           smallint     not null,
			    case_sensitive     smallint     not null,
			    searchable         smallint     not null,
			    unsigned_attribute smallint     null,
			    money              smallint     not null,
			    auto_increment     smallint     null,
			    local_type_name    varchar(128) not null,
			    aux                int          null)
go

/*
	There is a complicated set of SQL used to deal with
	the SQL Server Null data types (MONEYn, INTn, etc.)
	ISNULL is the only conditional SQL Server function that can be used
	to differentiate between these types depending on size.

	The aux column in the above table is used to differentiate
	the null data types from the non-null types.

	The aux column contains NULL for the null data types and 0
	for the non-null data types.

	The following SQL returns the contents of the aux column (0)
	for the non-null data types and returns a variable non-zero
	value for the null data types.

								 " I   I I FFMMDD"
								 " 1   2 4 484848"
	isnull(d.aux,ascii(substring("666AAA@@@CB??GG",
	2*(d.ss_dtype%35+1)+2-8/c.length,1))-60)

	The '2*(d.ss_dtype%35+1)+2-8/c.length' selects a specific character of
	the substring mask depending on the null data type and its size, i.e.
	null MONEY4 or null MONEY8.  The character selected is then converted
	to its binary value and an appropriate bias (i.e. 60) is subtracted to
	return the correct non-zero value.	This value may be used as a
	constant, i.e. ODBC data type, precision, scale, etc., or used as an
	index with a substring to pick out a character string, i.e. type name.

	The comments above the substring mask denote which character is
	selected for each null data type, i.e. In (INTn), Fn (FLOATn),
	Mn (MONEYn) and Dn (DATETIMn).
*/

grant select on spt_datatype_info to public
go

/* Get case sensitivity */
if 'A' = 'A' /* create dummy begin block */
begin
	declare @case smallint

	select @case = 0
	select @case = 1 where 'a' != 'A'

	/* Local Binary */
	insert into spt_datatype_info
	/* ss_type,name    ,data_type,prec,scale,rdx ,len ,prf ,suf ,cp      ,nul,case,srch,unsigned,money,auto,local   ,aux */ values
	(  45     ,"binary",-2       ,null,null ,null,null,"0x",null,"length",1  ,0   ,2   ,null    ,0    ,null,"binary",0)

	/* Local Bit */
	insert into spt_datatype_info
	/* ss_type,name ,data_type, prec,scale,rdx,len ,prf ,suf ,cp  ,nul,case,srch,unsigned,money,auto,local,aux */ values
	(  50     ,"bit",-7       ,	1   ,0    ,2  ,null,null,null,null,0  ,0   ,2   ,null    ,0    ,null,"bit",0)

	/* Local Char */
	insert into spt_datatype_info
	/* ss_type,name  ,data_type,prec,scale,rdx ,len ,prf,suf,cp      ,nul,case ,srch,unsigned,money,auto,local ,aux */ values
	(  47     ,"char",1        ,null,null ,null,null,"'","'","length",1  ,@case,3   ,null    ,0    ,null,"char",0)

	/* Local Datetime */
	insert into spt_datatype_info
	/* ss_type,name      ,data_type,prec,scale,rdx,len,prf,suf,cp  ,nul,case,srch,unsigned,money,auto,local     ,aux */ values
	(  61     ,"datetime",11       ,23  ,3    ,10 ,16 ,"'","'",null,1  ,0   ,3   ,null    ,0    ,null,"datetime",0)

	/* Local Smalldatetime */
	insert into spt_datatype_info
	/* ss_type,name           ,data_type,prec,scale,rdx,len,prf,suf,cp  ,nul,case,srch,unsigned,money,auto,local          ,aux */ values
	(  58     ,"smalldatetime",11       ,16  ,0    ,10 ,16 ,"'","'",null,1  ,0   ,3   ,null    ,0    ,null,"smalldatetime",0)

	/* Local Datetimn */
	insert into spt_datatype_info  /* sql server type is "datetimn" */
	/* ss_type,name           ,data_type,prec,scale,rdx,len,prf,suf,cp  ,nul,case,srch,unsigned,money,auto,local     ,aux */ values
	(  111    ,"smalldatetime",0        ,0   ,0    ,10 ,0  ,"'","'",null,1  ,0   ,3   ,null    ,0    ,null,"datetime",null)

	/* Local Float */
	insert into spt_datatype_info
	/* ss_type,name   ,data_type,prec,scale,rdx,len ,prf ,suf ,cp  ,nul,case,srch,unsigned,money,auto,local  ,aux */ values
	(  62     ,"float",6        ,15  ,null ,10 ,null,null,null,null,1  ,0   ,2   ,0       ,0    ,0   ,"float",0)

	/* Local RealFloat */
	insert into spt_datatype_info  /* sql server type is "floatn" */
	/* ss_type,name               ,data_type,prec,scale,rdx,len,prf ,suf ,cp  ,nul,case,srch,unsigned,money,auto,local            ,aux */ values
	(  109    ,"float        real",0        ,0   ,null ,10 ,0  ,null,null,null,1  ,0   ,2   ,0       ,0    ,0   ,"real      float",null)

	/* Local Real */
	insert into spt_datatype_info
	/* ss_type,name  ,data_type,prec,scale,rdx,len ,prf ,suf ,cp  ,nul,case,srch,unsigned,money,auto,local ,aux */ values
	(  59     ,"real",7        ,7   ,null ,10 ,null,null,null,null,1  ,0   ,2   ,0       ,0    ,0   ,"real",0)

	/* Local Smallmoney */
	insert into spt_datatype_info
	/* ss_type,name        ,data_type,prec,scale,rdx,len ,prf,suf ,cp  ,nul,case,srch,unsigned,money,auto,local       ,aux */ values
	(  122    ,"smallmoney",3        ,10  ,4    ,10 ,null,"$",null,null,1  ,0   ,2   ,0       ,1    ,0   ,"smallmoney",0)

	/* Local Int */
	insert into spt_datatype_info
	/* ss_type,name ,data_type,prec,scale,rdx,len ,prf ,suf ,cp  ,nul,case,srch,unsigned,money,auto,local,aux */ values
	(  56     ,"int",4        ,10  ,0    ,10 ,null,null,null,null,1  ,0   ,2   ,0       ,0    ,0   ,"int",0)

	/* Local Intn */
	insert into spt_datatype_info  /* sql server type is "intn" */
	/* ss_type,name                  ,data_type,prec,scale,rdx,len,prf ,suf ,cp  ,nul,case,srch,unsigned,money,auto,local               ,aux */ values
	(       38,"smallint     tinyint",0        ,0   ,0    ,10 ,0  ,null,null,null,1  ,0   ,2   ,0       ,0    ,0   ,"tinyint   smallint",null)

	/* Local Money */
	insert into spt_datatype_info
	/* ss_type,name   ,data_type,prec,scale,rdx,len ,prf,suf ,cp  ,nul,case,srch,unsigned,money,auto,local  ,aux */ values
	(  60     ,"money",3        ,19  ,4    ,10 ,null,"$",null,null,1  ,0   ,2   ,0       ,1    ,0   ,"money",0)

	/* Local Moneyn */
	insert into spt_datatype_info	/* sql server type is "moneyn" */
	/* ss_type,name        ,data_type,prec,scale,rdx,len,prf,suf ,cp  ,nul,case,srch,unsigned,money,auto,local            ,aux */ values
	(  110    ,"smallmoney",0        ,0   ,4    ,10 ,0  ,"$",null,null,1  ,0   ,2   ,0       ,1    ,0   ,"smallmoneymoney",null)

	/* Local Smallint */
	insert into spt_datatype_info
	/* ss_type,name      ,data_type,prec,scale,rdx,len ,prf ,suf ,cp  ,nul,case,srch,unsigned,money,auto,local     ,aux */ values
	(  52     ,"smallint",5        ,5   ,0    ,10 ,null,null,null,null,1  ,0   ,2   ,0       ,0    ,0   ,"smallint",0)

	/* Local Text */
	insert into spt_datatype_info
	/* ss_type,name  ,data_type,prec      ,scale,rdx ,len       ,prf,suf,cp  ,nul,case ,srch,unsigned,money,auto,local ,aux */ values
	(  35     ,"text",-1       ,2147483647,null ,null,2147483647,"'","'",null,1  ,@case,1   ,null    ,0    ,null,"text",0)

	/* Local Varbinary */
	insert into spt_datatype_info
	/* ss_type,name       ,data_type,prec,scale,rdx ,len ,prf ,suf ,cp          ,nul,case,srch,unsigned,money,auto,local      ,aux */ values
	(  37     ,"varbinary",-3       ,null,null ,null,null,"0x",null,"max length",1  ,0   ,2   ,null    ,0    ,null,"varbinary",0)

	/* Local Tinyint */
	insert into spt_datatype_info
	/* ss_type,name     ,data_type,prec,scale,rdx,len ,prf ,suf ,cp  ,nul,case,srch,unsigned,money,auto,local    ,aux */ values
	(  48     ,"tinyint",-6       ,3   ,0    ,10 ,null,null,null,null,1  ,0   ,2   ,1       ,0    ,0   ,"tinyint",0)

	/* Local Varchar */
	insert into spt_datatype_info
	/* ss_type,name     ,data_type,prec,scale,rdx ,len ,prf,suf,cp          ,nul,case ,srch,unsigned,money,auto,local    ,aux */ values
	(  39     ,"varchar",12       ,null,null ,null,null,"'","'","max length",1  ,@case,3   ,null    ,0    ,null,"varchar",0)

	/* Local Image */
	insert into spt_datatype_info
	/* ss_type,name   ,data_type,prec      ,scale,rdx ,len       ,prf ,suf ,cp  ,nul,case,srch,unsigned,money,auto,local  ,aux */ values
	(  34     ,"image",-4       ,2147483647,null ,null,2147483647,"0x",null,null,1  ,0   ,1   ,null    ,0    ,null,"image",0)
end
go

dump tran master with truncate_only
go

print "creating table spt_server_info"
go
create table spt_server_info (
			  attribute_id		int,
			  attribute_name	varchar(60),
			  attribute_value	varchar(255))
go

insert into spt_server_info
    values (1,"DBMS_NAME","Microsoft SQL Server")
insert into spt_server_info
    values (2,"DBMS_VER",@@version)
insert into spt_server_info
    values (10,"OWNER_TERM","owner")
insert into spt_server_info
    values (11,"TABLE_TERM","table")
insert into spt_server_info
    values (12,"MAX_OWNER_NAME_LENGTH","30")
insert into spt_server_info
    values (13,"TABLE_LENGTH","30")
insert into spt_server_info
    values (14,"MAX_QUAL_LENGTH","30")
insert into spt_server_info
    values (15,"COLUMN_LENGTH","30")
insert into spt_server_info
    values (16,"IDENTIFIER_CASE","MIXED")
insert into spt_server_info
    values (17,"TX_ISOLATION","2")
go
/*
** syscharsets doesn't exist on pre rel. 4.2 servers.
*/
if not exists (select * from sysobjects where name = 'syscharsets')
 if 'a' = 'A'	/* pre 4.2 case insensitive server */
        insert into spt_server_info values
           (18,"COLLATION_SEQ",
           'charset=cp850 sort_order=caseless_34 charset_num=2 sort_order_num=49')
     else			/* pre 4.2 case sensitive server */
        insert into spt_server_info values
           (18,"COLLATION_SEQ",
            'charset=cp850 sort_order=bin_cp850 charset_num=2 sort_order_num=40')
go

/*	Check if Version 1.11 */
if  (charindex('1.11', @@version) != 0)
   begin
    print ""
    print ""
    print "Installing catalog procedures on a 1.x or 4.0 server:"
    print "Ignore the following error messages"
    print ""
    print ""
   end
go

/*
** Use the insert that utilizes syscharsets if it exists.
*/
if exists (select * from sysobjects where name = 'syscharsets')
   insert into spt_server_info
      select 18,"COLLATION_SEQ",
         "charset="+t2.name+" sort_order="+t1.name
         +" charset_num="+rtrim(convert(char(4),t1.csid))+
         " sort_order_num="+rtrim(convert(char(4),t1.id))
     from syscharsets t1, syscharsets t2, sysconfigures t3
     where t1.csid=t2.id and t1.id=t3.value and t3.config=123
go

insert into spt_server_info
    values (19,"SAVEPOINT_SUPPORT","Y")
insert into spt_server_info
    values (20,"MULTI_RESULT_SETS","Y")
insert into spt_server_info
    values (22,"ACCESSIBLE_TABLES","Y")
insert into spt_server_info
    values (100,"USERID_LENGTH","30")
insert into spt_server_info
    values (101,"QUALIFIER_TERM","database")
insert into spt_server_info
    values (102,"NAMED_TRANSACTIONS","Y")
insert into spt_server_info
    values (103,"SPROC_AS_LANGUAGE","Y")
insert into spt_server_info
    values (104,"ACCESSIBLE_SPROC","Y")
insert into spt_server_info
    values (105,"MAX_INDEX_COLS","16")
insert into spt_server_info
    values (106,"RENAME_TABLE","Y")
insert into spt_server_info
    values (107,"RENAME_COLUMN","Y")
insert into spt_server_info
    values (108,"DROP_COLUMN","N")
insert into spt_server_info
    values (109,"INCREASE_COLUMN_LENGTH","N")
insert into spt_server_info
    values (110,"DDL_IN_TRANSACTION","N")
insert into spt_server_info
    values (111,"DESCENDING_INDEXES","N")
insert into spt_server_info
    values (112,"SP_RENAME","Y")
insert into spt_server_info
    values (113,"REMOTE_SPROC","Y")
insert into spt_server_info
	values (500,"SYS_SPROC_VERSION","02.00.4127")
go

grant select on spt_server_info to public
go

print "creating sp_tables"
go

create procedure sp_tables(
			   @table_name		varchar(32)  = null,
			   @table_owner     varchar(32)  = null,
			   @table_qualifier varchar(32)  = null,
			   @table_type		varchar(100) = null)
as
	declare @type1 varchar(3)
	declare @tableindex int


	/* Special feature #1:	enumerate databases when owner and name
		 are blank but qualifier is explicitly '%'.  */
	if @table_qualifier = '%' and
		@table_owner = '' and
		@table_name = ''
	begin	/* If enumerating databases */
		select
			table_qualifier = name,
			table_owner = convert(varchar(32),null),
			table_name = convert(varchar(32),null),
			table_type = convert(varchar(12),null),
			remarks = convert(varchar(254),null)	/* Remarks are NULL */
		from master..sysdatabases
		where name != 'model'	/* eliminate MODEL database */
		order by table_qualifier
	end

	/* Special feature #2:	enumerate owners when qualifier and name
		 are blank but owner is explicitly '%'.  */
	else if @table_qualifier = '' and
		@table_owner = '%' and
		@table_name = ''
	begin	/* If enumerating owners */
		select distinct
			table_qualifier = convert(varchar(32),null),
			table_owner = user_name(uid),
			table_name = convert(varchar(32),null),
			table_type = convert(varchar(12),null),
			remarks = convert(varchar(254),null)	/* Remarks are NULL */
		from sysobjects
		order by table_owner
	end

	/* Special feature #3:	enumerate table types when qualifier, owner and
		 name are blank but table type is explicitly '%'.	*/
	else if @table_qualifier = '' and
		@table_owner = '' and
		@table_name = '' and
		@table_type = '%'
	begin	/* If enumerating table types */
		select
			table_qualifier = convert(varchar(32),null),
			table_owner = convert(varchar(32),null),
			table_name = convert(varchar(32),null),
			table_type = rtrim(substring('SYSTEM TABLETABLE       VIEW',(colid-1)*12+1,12)),
			remarks = convert(varchar(254),null)	/* Remarks are NULL */
		from sysobjects o, syscolumns c
		where o.id=c.id and o.name='sysusers' and colid<=3
	end

	else
	begin /* end of special features -- do normal processing */
		if @table_qualifier is not null
		begin
			if db_name() != @table_qualifier
			begin
				if @table_qualifier = ''
				begin  /* If empty qualifier supplied */
					/* Force an empty result set */
					select @table_name = ''
					select @table_owner = ''
				end
				else
				begin	/* If qualifier doesn't match current database */
					print "Table qualifier must be name of current database"
					return
				end
			end
		end
		if @table_type is null
		begin	/* Select all ODBC supported table types */
			select @type1 = 'SUV'
		end
		else
		begin
			/*	TableType is case sensitive if CS server */
			select @type1 = null
			if (charindex("'SYSTEM TABLE'",@table_type) != 0)
				select @type1 = @type1 + 'S'	/* Add System Tables */
			if (charindex("'TABLE'",@table_type) != 0)
				select @type1 = @type1 + 'U'	/* Add User Tables */
			if (charindex("'VIEW'",@table_type) != 0)
				select @type1 = @type1 + 'V'	/* Add Views */
		end
		if @table_name is null
		begin	/*	If table name not supplied, match all */
			select @table_name = '%'
		end
		else
		begin
			if (@table_owner is null) and (charindex('%', @table_name) = 0)
			begin	/* If owner not specified and table is specified */
				if exists (select * from sysobjects
					where uid = user_id()
					and name = @table_name
					and (type = 'U' or type = 'V' or type = 'S'))
				begin	/* Override supplied owner w/owner of table */
					select @table_owner = user_name()
				end
			end
		end
		if @table_owner is null /* If no owner supplied, force wildcard */
			select @table_owner = '%'
		select
			table_qualifier = db_name(),
			table_owner = user_name(o.uid),
			table_name = o.name,
			table_type = rtrim(
				substring('SYSTEM TABLE            TABLE       VIEW       ',
					(ascii(o.type)-83)*12+1,12)),	/* 'S'=0,'U'=2,'V'=3 */
			remarks = convert(varchar(254),null)	/* Remarks are NULL */
		from sysusers u, sysobjects o
		where
			o.name like @table_name
			and user_name(o.uid) like @table_owner
			and charindex(substring(o.type,1,1),@type1)! = 0 /* Only desired types */
			and u.uid = user_id() /* constrain sysusers uid for use in subquery */
			and (
				suser_id() = 1	 /* User is the System Administrator */
				or o.uid = user_id()	 /* User created the object */
				/* here's the magic... select the highest precedence of permissions in the order (user,group,public)  */
				or ((select max(((sign(uid)*abs(uid-16383))*2)+(protecttype&1))
					from sysprotects p
					/* outer join to correlate with all rows in sysobjects */
					where p.id =* o.id
						/* get rows for public,current user,user's group */
						and (p.uid = 0 or p.uid = user_id() or p.uid =* u.gid)
						/* check for SELECT,EXECUTE privilege */
						and (action in (193,224)))&1	 /* more magic...normalize GRANT */
					) = 1	/* final magic...compare Grants	  */
			)
		order by table_type, table_qualifier, table_owner, table_name
	end
go

grant execute on sp_tables to public
go

dump tran master with truncate_only
go

print "creating sp_statistics"
go

/*	Check if Version 4.8 or 4.9 */
if  (charindex('4.8', @@version) = 0
    and charindex('4.9', @@version) = 0)
begin
	print ""
	print ""
	print "Warning:"
	print "you are installing the stored procedures "
	print "on SQL Server with version less than 4.8, "
	print "ignore the following error"
end
go

/*	This procedure is for 4.8 and 4.9 servers (catalog difference) */
CREATE PROCEDURE sp_statistics (
				 @table_name		varchar(32),
				 @table_owner		varchar(32) = null,
				 @table_qualifier	varchar(32) = null,
				 @index_name		varchar(32) = '%',
				 @is_unique 		char(1) = 'N',
				 @accuracy			char(1) = 'Q')
AS
	DECLARE @indid				int
	DECLARE @lastindid			int
	DECLARE @table_id			int
    DECLARE @full_table_name    char(70)

    if @table_qualifier is not null
    begin
		if db_name() != @table_qualifier
		begin	/* If qualifier doesn't match current database */
			print "Table qualifier must be name of current database"
			return
		end
    end

	if @accuracy not in ('Q','E')
	   begin
	      print "Invalid value - please enter either 'Q' or 'E' for accuracy."
		  return
	   end

    if @@trancount != 0
	begin	/* If inside a transaction */
		raiserror 20001 "stored procedure sp_statistics can not be run while in a transaction"
		return
    end
	create table #TmpIndex(
				table_qualifier varchar(32),
			    table_owner     varchar(32),
			    table_name      varchar(32),
			    index_qualifier varchar(32) null,
			    index_name      varchar(32) null,
			    non_unique      smallint null,
			    type            smallint,
			    seq_in_index    smallint null,
			    column_name     varchar(32) null,
			    collation       char(1) null,
			    index_id        int null,
			    cardinality     int null,
			    pages           int null,
			    status          smallint)
    if @table_owner is null
	begin	/* If unqualified table name */
		SELECT @full_table_name = @table_name
    end
    else
	begin	/* Qualified table name */
		SELECT @full_table_name = @table_owner + '.' + @table_name
    end
	/*	Get Object ID */
    SELECT @table_id = object_id(@full_table_name)

	/*	Start at lowest index id */
	SELECT @indid = min(indid)
    FROM sysindexes
	WHERE id = @table_id
		AND indid > 0
		AND indid < 255

    WHILE @indid != NULL
    BEGIN
		INSERT #TmpIndex	/* Add all columns that are in index */
			SELECT
				DB_NAME(),								/* table_qualifier */
				USER_NAME(o.uid),						/* table_owner	   */
				o.name, 								/* table_name	   */
				o.name, 								/* index_qualifier */
				x.name, 								/* index_name	   */
				0,										/* non_unique	   */
				1,										/* SQL_INDEX_CLUSTERED */
				colid,									/* seq_in_index    */
				INDEX_COL(@full_table_name,indid,colid),/* column_name	   */
				"A",									/* collation	   */
				@indid, 								/* index_id 	   */
				rowcnt(x.doampg),						/* cardinality	   */
				data_pgs(x.id,doampg),					/* pages		   */
				x.status								/* status		   */
			FROM sysindexes x, syscolumns c, sysobjects o
			WHERE
				x.id = @table_id
				AND x.id = o.id
				AND x.id = c.id
				AND c.colid < keycnt+(x.status&16)/16
				AND x.indid = @indid
		/*
		**	  Now move @indid to the next index.
		*/
		SELECT @lastindid = @indid
		SELECT @indid = NULL

		SELECT @indid = min(indid)
		FROM sysindexes
		WHERE id = @table_id
			AND indid > @lastindid
			AND indid < 255
    END

    UPDATE #TmpIndex
		SET non_unique = 1
		WHERE status&2 != 2 /* If non-unique index */
    UPDATE #TmpIndex
		SET
			type = 3,			/* SQL_INDEX_OTHER */
			cardinality = NULL,
			pages = NULL
		WHERE index_id > 1	/* If non-clustered index */

	/* now add row for table statistics */
	INSERT #TmpIndex
		SELECT
			DB_NAME(),				/* table_qualifier */
			USER_NAME(o.uid),		/* table_owner	   */
			o.name, 				/* table_name	   */
			null,					/* index_qualifier */
			null,					/* index_name	   */
			null,					/* non_unique	   */
			0,						/* SQL_TABLE_STAT  */
			null,					/* seq_in_index    */
			null,					/* column_name	   */
			null,					/* collation	   */
			0,						/* index_id 	   */
			rowcnt(x.doampg),		/* cardinality	   */
			data_pgs(x.id,doampg),	/* pages		   */
			0						/* status		   */
		FROM sysindexes x, sysobjects o
		WHERE o.id = @table_id
			AND x.id = o.id
			AND (x.indid = 0 or x.indid = 1)	/*	If there are no indexes */
												/*	then table stats are in */
												/*	a row with indid =0		*/

	if @is_unique != 'Y'	/* If all indexes desired */
		SELECT
			table_qualifier,
			table_owner,
			table_name,
			non_unique,
			index_qualifier,
			index_name,
			type,
			seq_in_index,
			column_name,
			collation,
			cardinality,
			pages,
			filter_condition = convert(varchar(128),null)
		FROM #TmpIndex
		WHERE
			index_name like @index_name /* If matching name */
			or index_name is null		/* If SQL_TABLE_STAT row */
		ORDER BY non_unique, type, index_name, seq_in_index
	else					/* If only unique indexes desired */
		SELECT
			table_qualifier,
			table_owner,
			table_name,
			non_unique,
			index_qualifier,
			index_name,
			type,
			seq_in_index,
			column_name,
			collation,
			cardinality,
			pages,
			filter_condition = convert(varchar(128),null)
		FROM #TmpIndex
		WHERE
			(non_unique = 0 			/* If unique */
				or non_unique is NULL)	/* If SQL_TABLE_STAT row */
			and (index_name like @index_name	/* If matching name */
				or index_name is null)	/* If SQL_TABLE_STAT row */
		ORDER BY non_unique, type, index_name, seq_in_index

    DROP TABLE #TmpIndex
go

if object_id('#TmpIndex') != null
	drop table #TmpIndex
dump tran master with truncate_only
go


if  (charindex('4.8', @@version) != 0
    or charindex('4.9', @@version) != 0)
begin
	print ""
	print ""
	print "Warning:"
	print "you are installing the stored procedures "
	print "on SQL Server with version 4.8 or greater. "
	print "Ignore the following error."
end
go

/*	This procedure is for < 4.8 servers */
CREATE PROCEDURE sp_statistics (
				 @table_name		varchar(32),
				 @table_owner		varchar(32) = null,
				 @table_qualifier	varchar(32) = null,
				 @index_name		varchar(32) = '%',
				 @is_unique 		char(1) = 'N',
				 @accuracy			char(1) = 'Q')
AS
	DECLARE @indid				int
	DECLARE @lastindid			int
	DECLARE @table_id			int
    DECLARE @full_table_name    char(70)

    if @table_qualifier is not null
    begin
		if db_name() != @table_qualifier
		begin	/* If qualifier doesn't match current database */
			print "Table qualifier must be name of current database"
			return
		end
    end

	if @accuracy not in ('Q','E')
	   begin
	      print "Invalid value - please enter either 'Q' or 'E' for accuracy."
		  return
	   end

    if @@trancount != 0
	begin	/* If inside a transaction */
		raiserror 20001 "stored procedure sp_statistics can not be run while in a transaction"
		return
    end
	create table #TmpIndex(
				table_qualifier varchar(32),
			    table_owner     varchar(32),
			    table_name      varchar(32),
			    index_qualifier varchar(32) null,
			    index_name      varchar(32) null,
			    non_unique      smallint null,
			    type            smallint,
			    seq_in_index    smallint null,
			    column_name     varchar(32) null,
			    collation       char(1) null,
			    index_id        int null,
			    cardinality     int null,
			    pages           int null,
			    status          smallint)
    if @table_owner is null
	begin	/* If unqualified table name */
		SELECT @full_table_name = @table_name
    end
    else
	begin	/* Qualified table name */
		SELECT @full_table_name = @table_owner + '.' + @table_name
    end
	/*	Get Object ID */
    SELECT @table_id = object_id(@full_table_name)

	/*	Start at lowest index id */
	SELECT @indid = min(indid)
    FROM sysindexes
	WHERE id = @table_id
		AND indid > 0
		AND indid < 255

    WHILE @indid != NULL
    BEGIN
		INSERT #TmpIndex	/* Add all columns that are in index */
			SELECT
				DB_NAME(),								/* table_qualifier */
				USER_NAME(o.uid),						/* table_owner	   */
				o.name, 								/* table_name	   */
				o.name, 								/* index_qualifier */
				x.name, 								/* index_name	   */
				0,										/* non_unique	   */
				1,										/* SQL_INDEX_CLUSTERED */
				colid,									/* seq_in_index    */
				INDEX_COL(@full_table_name,indid,colid),/* column_name	   */
				"A",									/* collation	   */
				@indid, 								/* index_id 	   */
				x.rows, 								/* cardinality	   */
				x.dpages,								/* pages		   */
				x.status								/* status			*/
			FROM sysindexes x, syscolumns c, sysobjects o
			WHERE
				x.id = @table_id
				AND x.id = o.id
				AND x.id = c.id
				AND c.colid < keycnt+(x.status&16)/16
				AND x.indid = @indid
		/*
		**	  Now move @indid to the next index.
		*/
		SELECT @lastindid = @indid
		SELECT @indid = NULL

		SELECT @indid = min(indid)
		FROM sysindexes
		WHERE id = @table_id
			AND indid > @lastindid
			AND indid < 255
    END

    UPDATE #TmpIndex
		SET non_unique = 1
		WHERE status&2 != 2 /* If non-unique index */
    UPDATE #TmpIndex
		SET
			type = 3,			/* SQL_INDEX_OTHER */
			cardinality = NULL,
			pages = NULL
		WHERE index_id > 1	/* If non-clustered index */

	/* now add row for table statistics */
	INSERT #TmpIndex
		SELECT
			DB_NAME(),				/* table_qualifier */
			USER_NAME(o.uid),		/* table_owner	   */
			o.name, 				/* table_name	   */
			null,					/* index_qualifier */
			null,					/* index_name	   */
			null,					/* non_unique	   */
			0,						/* SQL_TABLE_STAT  */
			null,					/* seq_in_index    */
			null,					/* column_name	   */
			null,					/* collation	   */
			0,						/* index_id 	   */
			x.rows, 				/* cardinality	   */
			x.dpages,				/* pages		   */
			0						/* status		   */
		FROM sysindexes x, sysobjects o
		WHERE o.id = @table_id
			AND x.id = o.id
			AND (x.indid = 0 or x.indid = 1)	/*	If there are no indexes */
												/*	then table stats are in */
												/*	a row with indid =0		*/

	if @is_unique != 'Y'	/* If all indexes desired */
		SELECT
			table_qualifier,
			table_owner,
			table_name,
			non_unique,
			index_qualifier,
			index_name,
			type,
			seq_in_index,
			column_name,
			collation,
			cardinality,
			pages,
			filter_condition = convert(varchar(128),null)
		FROM #TmpIndex
		WHERE
			index_name like @index_name /* If matching name */
			or index_name is null		/* If SQL_TABLE_STAT row */
		ORDER BY non_unique, type, index_name, seq_in_index
	else					/* If only unique indexes desired */
		SELECT
			table_qualifier,
			table_owner,
			table_name,
			non_unique,
			index_qualifier,
			index_name,
			type,
			seq_in_index,
			column_name,
			collation,
			cardinality,
			pages,
			filter_condition = convert(varchar(128),null)
		FROM #TmpIndex
		WHERE
			(non_unique = 0 			/* If unique */
				or non_unique is NULL)	/* If SQL_TABLE_STAT row */
			and (index_name like @index_name	/* If matching name */
				or index_name is null)	/* If SQL_TABLE_STAT row */
		ORDER BY non_unique, type, index_name, seq_in_index

    DROP TABLE #TmpIndex
go

grant execute on sp_statistics to public
go

dump tran master with truncate_only
go


print "creating sp_columns"
go

CREATE PROCEDURE sp_columns (
				 @table_name		varchar(32),
				 @table_owner		varchar(32) = null,
				 @table_qualifier	varchar(32) = null,
				 @column_name		varchar(32) = null )
AS
    DECLARE @full_table_name    char(70)
    DECLARE @table_id int

	if @column_name is null /*	If column name not supplied, match all */
		select @column_name = '%'
    if @table_qualifier is not null
    begin
		if db_name() != @table_qualifier
		begin	/* If qualifier doesn't match current database */
			print "Table qualifier must be name of current database"
			return
		end
    end
	if @table_name is null
	begin	/*	If table name not supplied, match all */
		select @table_name = '%'
	end
    if @table_owner is null
	begin	/* If unqualified table name */
		SELECT @full_table_name = @table_name
    end
    else
	begin	/* Qualified table name */
		SELECT @full_table_name = @table_owner + '.' + @table_name
    end

	/*	Get Object ID */
    SELECT @table_id = object_id(@full_table_name)
    if ((charindex('%',@full_table_name) = 0) and
		(charindex('_',@full_table_name) = 0)  and
		@table_id != 0)
    begin
		/* this block is for the case where there is no pattern
			 matching required for the table name */
		SELECT	/* INTn, FLOATn, DATETIMEn and MONEYn types */
			table_qualifier = DB_NAME(),
			table_owner = USER_NAME(o.uid),
			table_name = o.name,
			column_name = c.name,
			data_type = d.data_type+convert(smallint,
						isnull(d.aux,
						ascii(substring("666AAA@@@CB??GG",
						2*(d.ss_dtype%35+1)+2-8/c.length,1))
						-60)),
			type_name = rtrim(substring(d.type_name,
						1+isnull(d.aux,
						ascii(substring("III<<<MMMI<<A<A",
						2*(d.ss_dtype%35+1)+2-8/c.length,
						1))-60), 13)),
			precision = isnull(d.data_precision, convert(int,c.length))
						+isnull(d.aux, convert(int,
						ascii(substring("???AAAFFFCKFOLS",
						2*(d.ss_dtype%35+1)+2-8/c.length,1))-60)),
			length = isnull(d.length, convert(int,c.length)) +convert(int,
						isnull(d.aux,
						ascii(substring("AAA<BB<DDDHJSPP",
						2*(d.ss_dtype%35+1)+2-8/c.length,
						1))-64)),
			scale = d.numeric_scale +convert(smallint,
						isnull(d.aux,
						ascii(substring("<<<<<<<<<<<<<<?",
						2*(d.ss_dtype%35+1)+2-8/c.length,
						1))-60)),
			radix = d.numeric_radix,
			nullable =	/* set nullability from status flag */
				convert(smallint, convert(bit, c.status&8)),
			remarks = convert(varchar(254),null),	/* Remarks are NULL */
			ss_data_type = c.type,
			colid = c.colid
		FROM
			syscolumns c,
			sysobjects o,
			master.dbo.spt_datatype_info d,
			systypes t
		WHERE
			o.id = @table_id
			AND c.id = o.id
			AND c.type = d.ss_dtype
			AND c.name like @column_name
			AND d.ss_dtype IN (111, 109, 38, 110)	/* Just *N types */
			AND c.usertype < 100					/* No user defined types */
		UNION
		SELECT	/* All other types including user data types */
			table_qualifier = DB_NAME(),
			table_owner = USER_NAME(o.uid),
			table_name = o.name,
			column_name = c.name,
			data_type = convert(smallint,
				/*	Map systypes.type to ODBC type */
				/*		SS-Type 		"			    	 1	      "*/
				/*						"33 3 3     4 44 5 5 2 5 55666"*/
				/*						"45 7 9	    5 78 0 2 2 6 89012"*/
						ascii(substring("8;<9<H<<<<<:<=6<5<A<?<@<GC?GB",
						t.type%34+1,1))-60),
			type_name = t.name,
			precision = isnull(d.data_precision, convert(int,c.length))
						+isnull(d.aux, convert(int,
						ascii(substring("???AAAFFFCKFOLS",
						2*(d.ss_dtype%35+1)+2-8/c.length,1))-60)),
			length = isnull(d.length, convert(int,c.length)) +convert(int,
						isnull(d.aux,
						ascii(substring("AAA<BB<DDDHJSPP",
						2*(d.ss_dtype%35+1)+2-8/c.length,
						1))-64)),
			scale = d.numeric_scale +convert(smallint,
						isnull(d.aux,
						ascii(substring("<<<<<<<<<<<<<<?",
						2*(d.ss_dtype%35+1)+2-8/c.length,
						1))-60)),
			radix = d.numeric_radix,
			nullable =	/* set nullability from status flag */
				convert(smallint, convert(bit, c.status&8)),
			remarks = convert(varchar(254),null),	/* Remarks are NULL */
			ss_data_type = c.type,
			colid = c.colid
		FROM
			syscolumns c,
			sysobjects o,
			master.dbo.spt_datatype_info d,
			systypes t
		WHERE
			o.id = @table_id
			AND c.id = o.id
			AND c.type = d.ss_dtype
			AND c.usertype *= t.usertype
			AND c.name like @column_name
			AND (d.ss_dtype NOT IN (111, 109, 38, 110)	/* No *N types */
				OR c.usertype >= 100)					/* User defined types */
		ORDER BY colid
	end
	else
    begin
		/* this block is for the case where there IS pattern
			 matching done on the table name */
		if @table_owner is null /*	If owner not supplied, match all */
			select @table_owner = '%'
		SELECT	/* INTn, FLOATn, DATETIMEn and MONEYn types */
			table_qualifier = DB_NAME(),
			table_owner = USER_NAME(o.uid),
			table_name = o.name,
			column_name = c.name,
			data_type = d.data_type+convert(smallint,
						isnull(d.aux,
						ascii(substring("666AAA@@@CB??GG",
						2*(d.ss_dtype%35+1)+2-8/c.length,1))
						-60)),
			type_name = rtrim(substring(d.type_name,
						1+isnull(d.aux,
						ascii(substring("III<<<MMMI<<A<A",
						2*(d.ss_dtype%35+1)+2-8/c.length,
						1))-60), 13)),
			precision = isnull(d.data_precision, convert(int,c.length))
						+isnull(d.aux, convert(int,
						ascii(substring("???AAAFFFCKFOLS",
						2*(d.ss_dtype%35+1)+2-8/c.length,1))-60)),
			length = isnull(d.length, convert(int,c.length)) +convert(int,
						isnull(d.aux,
						ascii(substring("AAA<BB<DDDHJSPP",
						2*(d.ss_dtype%35+1)+2-8/c.length,
						1))-64)),
			scale = d.numeric_scale +convert(smallint,
						isnull(d.aux,
						ascii(substring("<<<<<<<<<<<<<<?",
						2*(d.ss_dtype%35+1)+2-8/c.length,
						1))-60)),
			radix = d.numeric_radix,
			nullable =	/* set nullability from status flag */
				convert(smallint, convert(bit, c.status&8)),
			remarks = convert(varchar(254),null),	/* Remarks are NULL */
			ss_data_type = c.type,
			colid = c.colid
		FROM
			syscolumns c,
			sysobjects o,
			master.dbo.spt_datatype_info d,
			systypes t
		WHERE
			o.name like @table_name
			AND user_name(o.uid) like @table_owner
			AND o.id = c.id
			AND c.type = d.ss_dtype
			AND o.type != 'P'
			AND c.name like @column_name
			AND d.ss_dtype IN (111, 109, 38, 110)	/* Just *N types */
		UNION
		SELECT	/* All other types including user data types */
			table_qualifier = DB_NAME(),
			table_owner = USER_NAME(o.uid),
			table_name = o.name,
			column_name = c.name,
			data_type = convert(smallint,
				/*	Map systypes.type to ODBC type */
				/*		SS-Type 		"		    		 1	      "*/
				/*						"33 3 3     4 44 5 5 2 5 55666"*/
				/*						"45 7 9	    5 78 0 2 2 6 89012"*/
						ascii(substring("8;<9<H<<<<<:<=6<5<A<?<@<GC?GB",
						t.type%34+1,1))-60),
			type_name = t.name,
			precision = isnull(d.data_precision, convert(int,c.length))
						+isnull(d.aux, convert(int,
						ascii(substring("???AAAFFFCKFOLS",
						2*(d.ss_dtype%35+1)+2-8/c.length,1))-60)),
			length = isnull(d.length, convert(int,c.length)) +convert(int,
						isnull(d.aux,
						ascii(substring("AAA<BB<DDDHJSPP",
						2*(d.ss_dtype%35+1)+2-8/c.length,
						1))-64)),
			scale = d.numeric_scale +convert(smallint,
						isnull(d.aux,
						ascii(substring("<<<<<<<<<<<<<<?",
						2*(d.ss_dtype%35+1)+2-8/c.length,
						1))-60)),
			radix = d.numeric_radix,
			nullable =	/* set nullability from status flag */
				convert(smallint, convert(bit, c.status&8)),
			remarks  = convert(varchar(254),null),
			ss_data_type = c.type,
			colid = c.colid
		FROM
			syscolumns c,
			sysobjects o,
			master.dbo.spt_datatype_info d,
			systypes t
		WHERE
			o.name like @table_name
			AND user_name(o.uid) like @table_owner
			AND o.id = c.id
			AND c.type = d.ss_dtype
			AND c.usertype *= t.usertype
			AND o.type != 'P'
			AND c.name like @column_name
			AND d.ss_dtype NOT IN (111, 109, 38, 110)	/* No *N types */
		ORDER BY table_owner, table_name, colid
	end
go

grant execute on sp_columns to public
go

dump tran master with truncate_only
go
print "creating sp_fkeys"
go

CREATE PROCEDURE sp_fkeys(
			   @pktable_name		varchar(32) = null,
			   @pktable_owner		varchar(32) = null,
			   @pktable_qualifier	varchar(32) = null,
			   @fktable_name		varchar(32) = null,
			   @fktable_owner		varchar(32) = null,
			   @fktable_qualifier	varchar(32) = null )
as
    declare	@order_by_pk int

    select  @order_by_pk = 0

    if (@pktable_name is null) and (@fktable_name is null)
	begin	/* If neither primary key nor foreign key table names given */
		print "pk table name or fk table name must be given"
		return
    end
    if @fktable_qualifier is not null
    begin
		if db_name() != @fktable_qualifier
		begin	/* If qualifier doesn't match current database */
			print "Foreign Key Table qualifier must be name of current database"
			return
		end
    end
    if @pktable_qualifier is not null
    begin
		if db_name() != @pktable_qualifier
		begin	/* If qualifier doesn't match current database */
			print "Primary Key Table qualifier must be name of current database"
			return
		end
    end

    if @pktable_name is null
	begin /*  If table name not supplied, match all */
		select @pktable_name = '%'
		select @order_by_pk = 1
    end
	if @pktable_owner is null	/*	If PK owner not supplied, match all */
		select @pktable_owner = '%'
	if @fktable_name is null	/*	If table name not supplied, match all */
		select @fktable_name = '%'
	if @fktable_owner is null	/*	If FK owner not supplied, match all */
		select @fktable_owner = '%'

    if @@trancount != 0
	begin	/* If inside a transaction */
		raiserror 20001 "catalog procedure sp_fkeys can not be run in a transaction"
	return
    end
	create table #fkeys(
			 pktable_qualifier	varchar(32),
			 pktable_owner		varchar(32),
			 pktable_name       varchar(32),
			 pkcolumn_name      varchar(32),
			 fktable_qualifier	varchar(32),
			 fktable_owner		varchar(32),
			 fktable_name       varchar(32),
			 fkcolumn_name		varchar(32),
			 key_seq			smallint)

	/*	SQL Server supports upto 8 PK/FK relationships between 2 tables */
	/*	Process syskeys for each relationship */
	/*	The inserts below adds a row to the temp table for each of the
		8 possible relationships */
    insert into #fkeys
		select
			db_name(),
			user_name(o1.uid),
			object_name(k.depid),
			c2.name,
			db_name(),
			user_name(o2.uid),
			object_name(k.id),
			c1.name,
			1
		from
			syskeys k, syscolumns c1, syscolumns c2,sysobjects o1, sysobjects o2
		where
			c1.id = k.id
			and k.type = 2	/* Foreign type key */
			and c1.colid = k.key1
			and c2.id = k.depid
			and c2.colid = k.depkey1
			and o1.id = k.depid
			and o2.id = k.id
	union
	    select
			db_name(),
			user_name(o1.uid),
			object_name(k.depid),
			c2.name,
			db_name(),
			user_name(o2.uid),
			object_name(k.id),
			c1.name,
			2
		from
			syskeys k, syscolumns c1, syscolumns c2,sysobjects o1, sysobjects o2
		where
			c1.id = k.id
			and k.type = 2	/* Foreign type key */
			and c1.colid = k.key2
			and c2.id = k.depid
			and c2.colid = k.depkey2
			and o1.id = k.depid
			and o2.id = k.id
	union
		select
			db_name(),
			user_name(o1.uid),
			object_name(k.depid),
			c2.name,
			db_name(),
			user_name(o2.uid),
			object_name(k.id),
			c1.name,
			3
		from
			syskeys k, syscolumns c1, syscolumns c2,sysobjects o1, sysobjects o2
		where
			c1.id = k.id
			and k.type = 2	/* Foreign type key */
			and c1.colid = k.key3
			and c2.id = k.depid
			and c2.colid = k.depkey3
			and o1.id = k.depid
			and o2.id = k.id
	union
		select
			db_name(),
			user_name(o1.uid),
			object_name(k.depid),
			c2.name,
			db_name(),
			user_name(o2.uid),
			object_name(k.id),
			c1.name,
			4
		from
			syskeys k, syscolumns c1, syscolumns c2,sysobjects o1, sysobjects o2
		where
			c1.id = k.id
			and k.type = 2	/* Foreign type key */
			and c1.colid = k.key4
			and c2.id = k.depid
			and c2.colid = k.depkey4
			and o1.id = k.depid
			and o2.id = k.id
	union
		select
			db_name(),
			user_name(o1.uid),
			object_name(k.depid),
			c2.name,
			db_name(),
			user_name(o2.uid),
			object_name(k.id),
			c1.name,
			5
		from
			syskeys k, syscolumns c1, syscolumns c2,sysobjects o1, sysobjects o2
		where
			c1.id = k.id
			and k.type = 2	/* Foreign type key */
			and c1.colid = k.key5
			and c2.id = k.depid
			and c2.colid = k.depkey5
			and o1.id = k.depid
			and o2.id = k.id
	union
		select
			db_name(),
			user_name(o1.uid),
			object_name(k.depid),
			c2.name,
			db_name(),
			user_name(o2.uid),
			object_name(k.id),
			c1.name,
			6
		from
			syskeys k, syscolumns c1, syscolumns c2,sysobjects o1, sysobjects o2
		where
			c1.id = k.id
			and k.type = 2	/* Foreign type key */
			and c1.colid = k.key6
			and c2.id = k.depid
			and c2.colid = k.depkey6
			and o1.id = k.depid
			and o2.id = k.id
	union
	    select
			db_name(),
			user_name(o1.uid),
			object_name(k.depid),
			c2.name,
			db_name(),
			user_name(o2.uid),
			object_name(k.id),
			c1.name,
			7
		from
			syskeys k, syscolumns c1, syscolumns c2,sysobjects o1, sysobjects o2
		where
			c1.id = k.id
			and k.type = 2	/* Foreign type key */
			and c1.colid = k.key7
			and c2.id = k.depid
			and c2.colid = k.depkey7
			and o1.id = k.depid
			and o2.id = k.id
	union
	    select
			db_name(),
			user_name(o1.uid),
			object_name(k.depid),
			c2.name,
			db_name(),
			user_name(o2.uid),
			object_name(k.id),
			c1.name,
			8
		from
			syskeys k, syscolumns c1, syscolumns c2,sysobjects o1, sysobjects o2
		where
			c1.id = k.id
			and k.type = 2	/* Foreign type key */
			and c1.colid = k.key8
			and c2.id = k.depid
			and c2.colid = k.depkey8
			and o1.id = k.depid
			and o2.id = k.id

	if @order_by_pk = 1 /*	If order by PK fields */
		select
			pktable_qualifier,
			pktable_owner,
			pktable_name,
			pkcolumn_name,
			fktable_qualifier,
			fktable_owner,
			fktable_name,
			fkcolumn_name,
			key_seq,
			update_rule = convert(smallint, null),
			delete_rule = convert(smallint,null),
			fk_name = convert(varchar(32),null),
			pk_name = convert(varchar(32),null)
		from #fkeys
		where fktable_name like @fktable_name
			and fktable_owner like @fktable_owner
			and pktable_name  like @pktable_name
			and pktable_owner like @pktable_owner
		order by pktable_qualifier, pktable_owner, pktable_name, key_seq
	else		/*	Order by FK fields */
		select
			pktable_qualifier,
			pktable_owner,
			pktable_name,
			pkcolumn_name,
			fktable_qualifier,
			fktable_owner,
			fktable_name,
			fkcolumn_name,
			key_seq,
			update_rule = convert(smallint,null),
			delete_rule = convert(smallint,null),
			fk_name = convert(varchar(32),null),
			pk_name = convert(varchar(32),null)
		from #fkeys
		where fktable_name like @fktable_name
			and fktable_owner like @fktable_owner
			and pktable_name  like @pktable_name
			and pktable_owner like @pktable_owner
		order by fktable_qualifier, fktable_owner, fktable_name, key_seq
go

grant execute on sp_fkeys to public
go
dump tran master with truncate_only
go

print "creating sp_pkeys"
go

CREATE PROCEDURE sp_pkeys(
			   @table_name		varchar(32),
			   @table_owner 	varchar(32) = null,
			   @table_qualifier varchar(32) = null )
as
    if @table_qualifier is not null
    begin
		if db_name() != @table_qualifier
		begin	/* If qualifier doesn't match current database */
			print "Table qualifier must be name of current database"
			return
		end
    end
	if @table_owner is null /*	If owner not supplied, match all */
		select @table_owner = '%'
    if @@trancount != 0
	begin	/* If inside a transaction */
		raiserror 20001 "catalog procedure sp_pkeys can not be run in a transaction"
		return
    end

	create table #pkeys(
			 table_qualifier varchar(32),
			 table_owner     varchar(32),
			 table_name      varchar(32),
			 column_name     varchar(32),
			 key_seq		 smallint)

	/*	SQL Server supports upto 8 PK/FK relationships between 2 tables */
	/*	Process syskeys for each relationship */
	/*	The inserts below adds a row to the temp table for each of the
		8 possible relationships */
    insert into #pkeys
		select
			db_name(),
			(select user_name(uid) from sysobjects o where o.id = k.id),
			object_name(k.id),
			c.name,
			1
		from
			syskeys k, syscolumns c
		where
			c.id = k.id
			and k.type = 1	/* Primary type key */
			and c.colid = k.key1
    if (@@rowcount = 0)
		goto done

    insert into #pkeys
		select
			db_name(),
			(select user_name(uid) from sysobjects o where o.id = k.id),
			object_name(k.id),
			c.name,
			2
		from
			syskeys k, syscolumns c
		where
			c.id = k.id
			and k.type = 1	/* Primary type key */
			and c.colid = key2
    if (@@rowcount = 0)
		goto done

    insert into #pkeys
		select
			db_name(),
			(select user_name(uid) from sysobjects o where o.id = k.id),
			object_name(k.id),
			c.name,
			3
		from
			syskeys k, syscolumns c
		where
			c.id = k.id
			and k.type = 1	/* Primary type key */
			and c.colid = key3
    if (@@rowcount = 0)
		goto done

    insert into #pkeys
		select
			db_name(),
			(select user_name(uid) from sysobjects o where o.id = k.id),
			object_name(k.id),
			c.name,
			4
		from
			syskeys k, syscolumns c
		where
			c.id = k.id
			and k.type = 1	/* Primary type key */
			and c.colid = key4
    if (@@rowcount = 0)
		goto done

    insert into #pkeys
		select
			db_name(),
			(select user_name(uid) from sysobjects o where o.id = k.id),
			object_name(k.id),
			c.name,
			5
		from
			syskeys k, syscolumns c
		where
			c.id = k.id
			and k.type = 1	/* Primary type key */
			and c.colid = key5
    if (@@rowcount = 0)
		goto done

    insert into #pkeys
		select
			db_name(),
			(select user_name(uid) from sysobjects o where o.id = k.id),
			object_name(k.id),
			c.name,
			6
		from
			syskeys k, syscolumns c
		where
			c.id = k.id
			and k.type = 1	/* Primary type key */
			and c.colid = key6
    if (@@rowcount = 0)
		goto done

    insert into #pkeys
		select
			db_name(),
			(select user_name(uid) from sysobjects o where o.id = k.id),
			object_name(k.id),
			c.name,
			7
		from
			syskeys k, syscolumns c
		where
			c.id = k.id
			and k.type = 1	/* Primary type key */
			and c.colid = key7
    if (@@rowcount = 0)
		goto done

    insert into #pkeys
		 select
			 db_name(),
			 (select user_name(uid) from sysobjects o where o.id = k.id),
			 object_name(k.id),
			 c.name,
			 8
		 from
			 syskeys k, syscolumns c
		 where
			 c.id = k.id
			 and k.type = 1 /* Primary type key */
			 and c.colid = key8

    done:
    select
	    table_qualifier,
	    table_owner,
	    table_name,
	    column_name,
	    key_seq,
		pk_name = convert(varchar(32),null)
	from #pkeys
    where table_name like  @table_name
		and table_owner like @table_owner
	order by table_qualifier, table_owner, table_name, key_seq
go

grant execute on sp_pkeys to public
go

dump tran master with truncate_only
go

print "creating sp_stored_procedures"
go

create procedure sp_stored_procedures(
						@sp_name		varchar(36) = null,
						@sp_owner		varchar(32) = null,
						@sp_qualifier	varchar(32) = null)
as
	declare @proc_type smallint

    if @sp_qualifier is not null
    begin
		if db_name() != @sp_qualifier
		begin
			if @sp_qualifier = ''
			begin
				/* in this case, we need to return an empty result set */
				/* because the user has requested a database with an empty name */
				select @sp_name = ''
				select @sp_owner = ''
			end else
			begin	/* If qualifier doesn't match current database */
				print "Stored procedure qualifier must be name of current database"
				return
			end
		end
    end

    if @sp_name is null
	begin  /*  If procedure name not supplied, match all */
		select @sp_name = '%'
    end
	else begin
		if (@sp_owner is null) and (charindex('%', @sp_name) = 0)
		begin
			if exists (select * from sysobjects
				where uid = user_id()
					and name = @sp_name
					and type = 'P') /* Object type of Procedure */
			begin
				select @sp_owner = user_name()
			end
		end
    end
	if @sp_owner is null	/*	If procedure owner not supplied, match all */
		select @sp_owner = '%'

	if  not (charindex('1.1', @@version) = 0
    		and charindex('4.0', @@version) = 0)
    		select @proc_type=1		/* Return 1 for pre 4.2 servers. */
    else
    		select @proc_type=2		/* Return 2 for 4.2 and later servers. */

    select
	    procedure_qualifier = db_name(),
	    procedure_owner = user_name(o.uid),
	    procedure_name = o.name +';'+ ltrim(str(p.number,5)),
		num_input_params = -1,	/* Constant since value unknown */
		num_output_params = -1, /* Constant since value unknown */
		num_result_sets = -1,	/* Constant since value unknown */
		remarks = convert(varchar(254),null),	/* Remarks are NULL */
		procedure_type = @proc_type
	from
	    sysobjects o,sysprocedures p,sysusers u
	where
	    o.name like @sp_name
	    and p.sequence = 0
	    and user_name(o.uid) like @sp_owner
		and o.type = 'P'		/* Object type of Procedure */
	    and p.id = o.id
		and u.uid = user_id()	/* constrain sysusers uid for use in subquery */
		and (suser_id() = 1 	/* User is the System Administrator */
			or o.uid = user_id()	/* User created the object */
		    /* here's the magic... select the highest precedence of permissions in the order (user,group,public)  */
		    or ((select max(((sign(uid)*abs(uid-16383))*2)+(protecttype&1))
			 from sysprotects p
			 /* outer join to correlate with all rows in sysobjects */
			 where p.id =* o.id
			     /*  get rows for public,current user,user's group */
				 and (p.uid = 0 or p.uid = user_id() or p.uid =* u.gid)
			     /* check for SELECT,EXECUTE privilege */
			     and (action in (193,224)))&1    /* more magic...normalize GRANT */
			) = 1	 /* final magic...compare Grants	*/
		)
    order by procedure_qualifier, procedure_owner, procedure_name
go
grant execute on sp_stored_procedures to public
go

dump tran master with truncate_only
go


print "creating sp_sproc_columns"
go

CREATE PROCEDURE sp_sproc_columns (
				 @procedure_name		varchar(36) = '%',
				 @procedure_owner		varchar(32) = null,
				 @procedure_qualifier	varchar(32) = null,
				 @column_name			varchar(32) = null )
AS
    DECLARE @group_num int
    DECLARE @semi_position int
    DECLARE @full_procedure_name    char(70)
    DECLARE @procedure_id int

	if @column_name is null /*	If column name not supplied, match all */
		select @column_name = '%'
	if @procedure_qualifier is not null
    begin
		if db_name() != @procedure_qualifier
		begin
			if @procedure_qualifier = ''
			begin
				/* in this case, we need to return an empty result set */
				/* because the user has requested a database with an empty name */
				select @procedure_name = ''
				select @procedure_owner = ''
			end
			else
			begin	/* If qualifier doesn't match current database */
				print "Procedure qualifier must be name of current database"
				return
			end
		end
    end


	/* first we need to extract the procedure group number, if one exists */
    select @semi_position = charindex(';',@procedure_name)
	if (@semi_position > 0)
	begin	/* If group number separator (;) found */
		select @group_num = convert(int,substring(@procedure_name, @semi_position + 1, 2))
		select @procedure_name = substring(@procedure_name, 1, @semi_position -1)
    end
    else
	begin	/* No group separator, so default to group number of 1 */
		select @group_num = 1
    end

    if @procedure_owner is null
	begin	/* If unqualified procedure name */
		SELECT @full_procedure_name = @procedure_name
    end
    else
	begin	/* Qualified procedure name */
		SELECT @full_procedure_name = @procedure_owner + '.' + @procedure_name
    end

	/*	Get Object ID */
    SELECT @procedure_id = object_id(@full_procedure_name)
    if ((charindex('%',@full_procedure_name) = 0) and
		(charindex('_',@full_procedure_name) = 0)  and
		@procedure_id != 0)
    begin
		/* this block is for the case where there is no pattern
			matching required for the procedure name */
		SELECT	/* INTn, FLOATn, DATETIMEn and MONEYn types */
			procedure_qualifier = DB_NAME(),
			procedure_owner = USER_NAME(o.uid),
			procedure_name = o.name +';'+ ltrim(str(c.number,5)),
			column_name = c.name,
			column_type = convert(smallint, 0),
			data_type = d.data_type+convert(smallint,
						isnull(d.aux,
						ascii(substring("666AAA@@@CB??GG",
						2*(d.ss_dtype%35+1)+2-8/c.length,1))
						-60)),
			type_name = rtrim(substring(d.type_name,
						1+isnull(d.aux,
						ascii(substring("III<<<MMMI<<A<A",
						2*(d.ss_dtype%35+1)+2-8/c.length,
						1))-60), 13)),
			precision = isnull(d.data_precision, convert(int,c.length))
						+isnull(d.aux, convert(int,
						ascii(substring("???AAAFFFCKFOLS",
						2*(d.ss_dtype%35+1)+2-8/c.length,1))-60)),
			length = isnull(d.length, convert(int,c.length)) +convert(int,
						isnull(d.aux,
						ascii(substring("AAA<BB<DDDHJSPP",
						2*(d.ss_dtype%35+1)+2-8/c.length,
						1))-64)),
			scale = d.numeric_scale +convert(smallint,
						isnull(d.aux,
						ascii(substring("<<<<<<<<<<<<<<?",
						2*(d.ss_dtype%35+1)+2-8/c.length,
						1))-60)),
			radix = d.numeric_radix,
			nullable = convert(smallint,t.allownulls),
			remarks = convert(varchar(254),null),	/* Remarks are NULL */
			ss_data_type = c.type,
			colid = c.colid
		FROM
			syscolumns c,
			sysobjects o,
			master.dbo.spt_datatype_info d,
			systypes t,
			sysprocedures p
		WHERE
			o.id = @procedure_id
			AND c.id = o.id
			AND c.usertype = t.usertype
			AND c.type = d.ss_dtype
			AND c.name like @column_name
			AND d.ss_dtype IN (111, 109, 38, 110)	/* Just *N types */
			AND c.number = @group_num
		UNION
		SELECT		   /* All other types including user data types */
			procedure_qualifier = DB_NAME(),
			procedure_owner = USER_NAME(o.uid),
			procedure_name = o.name +';'+ ltrim(str(c.number,5)),
			column_name = c.name,
			column_type = convert(smallint, 0),
			data_type = convert(smallint,
				/*	Map systypes.type to ODBC type */
				/*		SS-Type 		"		    		 1	      "*/
				/*						"33 3 3     4 44 5 5 2 5 55666"*/
				/*						"45 7 9	    5 78 0 2 2 6 89012"*/
						ascii(substring("8;<9<H<<<<<:<=6<5<A<?<@<GC?GB",
						t.type%34+1,1))-60),
			type_name = t.name,
			precision = isnull(d.data_precision, convert(int,c.length))
						+isnull(d.aux, convert(int,
						ascii(substring("???AAAFFFCKFOLS",
						2*(d.ss_dtype%35+1)+2-8/c.length,1))-60)),
			length = isnull(d.length, convert(int,c.length)) +convert(int,
						isnull(d.aux,
						ascii(substring("AAA<BB<DDDHJSPP",
						2*(d.ss_dtype%35+1)+2-8/c.length,
						1))-64)),
			scale = d.numeric_scale +convert(smallint,
						isnull(d.aux,
						ascii(substring("<<<<<<<<<<<<<<?",
						2*(d.ss_dtype%35+1)+2-8/c.length,
						1))-60)),
			radix = d.numeric_radix,
			nullable = convert(smallint,t.allownulls),
			remarks = convert(varchar(254),null),	/* Remarks are NULL */
			ss_data_type = c.type,
			colid = c.colid
		FROM
			syscolumns c,
			sysobjects o,
			master.dbo.spt_datatype_info d,
			systypes t
		WHERE
			o.id = @procedure_id
			AND c.id = o.id
			AND c.type = d.ss_dtype
			AND c.usertype *= t.usertype
			AND c.name like @column_name
			AND d.ss_dtype NOT IN (111, 109, 38, 110)	/* No *N types */
			AND c.number = @group_num
		UNION
		SELECT		   /* return value row*/
			procedure_qualifier = DB_NAME(),
			procedure_owner = USER_NAME(o.uid),
			procedure_name = o.name +';'+ ltrim(str(c.number,5)),
			column_name = convert(varchar(32),'RETURN_VALUE'),
			column_type = convert(smallint, 5),
			data_type = convert(smallint, 4),
			type_name = convert(varchar(32),'int'),
			precision = convert(int,10),
			length = convert(int,4),
			scale = convert(smallint,0),
			radix = convert(smallint,10),
			nullable = convert(smallint,0),
			remarks = convert(varchar(254),null),	/* Remarks are NULL */
			ss_data_type = convert(tinyint,56),
			colid = convert(tinyint,0)
		FROM
			syscolumns c,
			sysobjects o
		WHERE
			o.id = @procedure_id
			AND c.id = o.id
			AND 'RETURN_VALUE' like @column_name
		ORDER BY procedure_qualifier, procedure_owner, procedure_name, column_type, colid
	end
	else
    begin
		/* this block is for the case where there IS pattern
			matching done on the procedure name */
		if @procedure_owner is null
			select @procedure_owner = '%'
		SELECT	/* INTn, FLOATn, DATETIMEn and MONEYn types */
			procedure_qualifier = DB_NAME(),
			procedure_owner = USER_NAME(o.uid),
			procedure_name = o.name +';'+ ltrim(str(c.number,5)),
			column_name = c.name,
			column_type = convert(smallint, 0),
			data_type = d.data_type+convert(smallint,
						isnull(d.aux,
						ascii(substring("666AAA@@@CB??GG",
						2*(d.ss_dtype%35+1)+2-8/c.length,1))
						-60)),
			type_name = rtrim(substring(d.type_name,
						1+isnull(d.aux,
						ascii(substring("III<<<MMMI<<A<A",
						2*(d.ss_dtype%35+1)+2-8/c.length,
						1))-60), 13)),
			precision = isnull(d.data_precision, convert(int,c.length))
						+isnull(d.aux, convert(int,
						ascii(substring("???AAAFFFCKFOLS",
						2*(d.ss_dtype%35+1)+2-8/c.length,1))-60)),
			length = isnull(d.length, convert(int,c.length)) +convert(int,
						isnull(d.aux,
						ascii(substring("AAA<BB<DDDHJSPP",
						2*(d.ss_dtype%35+1)+2-8/c.length,
						1))-64)),
			scale = d.numeric_scale +convert(smallint,
						isnull(d.aux,
						ascii(substring("<<<<<<<<<<<<<<?",
						2*(d.ss_dtype%35+1)+2-8/c.length,
						1))-60)),
			radix = d.numeric_radix,
			nullable = convert(smallint,t.allownulls),
			remarks = convert(varchar(254),null),	/* Remarks are NULL */
			ss_data_type = c.type,
			colid = c.colid
		FROM
			syscolumns c,
			sysobjects o,
			master.dbo.spt_datatype_info d,
			systypes t
		WHERE
			o.name like @procedure_name
			AND user_name(o.uid) like @procedure_owner
			AND o.id = c.id
			AND c.usertype = t.usertype
			AND c.type = d.ss_dtype
			AND c.name like @column_name
			AND o.type = 'P'						/* Just Procedures */
			AND d.ss_dtype IN (111, 109, 38, 110)	/* Just *N types */
		UNION
		SELECT		   /* All other types including user data types */
			procedure_qualifier = DB_NAME(),
			procedure_owner = USER_NAME(o.uid),
			procedure_name = o.name +';'+ ltrim(str(c.number,5)),
			column_name = c.name,
			column_type = convert(smallint, 0),
			data_type = convert(smallint,
				/*	Map systypes.type to ODBC type */
				/*		SS-Type 		"		    		 1	      "*/
				/*						"33 3 3     4 44 5 5 2 5 55666"*/
				/*						"45 7 9	    5 78 0 2 2 6 89012"*/
						ascii(substring("8;<9<H<<<<<:<=6<5<A<?<@<GC?GB",
						t.type%34+1,1))-60),
			type_name = t.name,
			precision = isnull(d.data_precision, convert(int,c.length))
						+isnull(d.aux, convert(int,
						ascii(substring("???AAAFFFCKFOLS",
						2*(d.ss_dtype%35+1)+2-8/c.length,1))-60)),
			length = isnull(d.length, convert(int,c.length)) +convert(int,
						isnull(d.aux,
						ascii(substring("AAA<BB<DDDHJSPP",
						2*(d.ss_dtype%35+1)+2-8/c.length,
						1))-64)),
			scale = d.numeric_scale +convert(smallint,
						isnull(d.aux,
						ascii(substring("<<<<<<<<<<<<<<?",
						2*(d.ss_dtype%35+1)+2-8/c.length,
						1))-60)),
			radix = d.numeric_radix,
			nullable = convert(smallint,t.allownulls),
			remarks = convert(varchar(254),null),	/* Remarks are NULL */
			ss_data_type = c.type,
			colid = c.colid
		FROM
			syscolumns c,
			sysobjects o,
			master.dbo.spt_datatype_info d,
			systypes t
		WHERE
			o.name like @procedure_name
			AND user_name(o.uid) like @procedure_owner
			AND o.id = c.id
			AND c.type = d.ss_dtype
			AND c.usertype *= t.usertype
			AND o.type = 'P'							/* Just Procedures */
			AND c.name like @column_name
			AND d.ss_dtype NOT IN (111, 109, 38, 110)	/* No *N types */
		UNION
		SELECT		   /* return value row*/
			procedure_qualifier = DB_NAME(),
			procedure_owner = USER_NAME(o.uid),
			procedure_name = o.name +';'+ ltrim(str(c.number,5)),
			column_name = convert(varchar(32),'RETURN_VALUE'),
			column_type = convert(smallint, 5),
			data_type = convert(smallint, 4),
			type_name = convert(varchar(32),'int'),
			precision = convert(int,10),
			length = convert(int,4),
			scale = convert(smallint,0),
			radix = convert(smallint,10),
			nullable = convert(smallint,0),
			remarks = convert(varchar(254),null),	/* Remarks are NULL */
			ss_data_type = convert(tinyint,56),
			colid = convert(tinyint,0)
		FROM
			syscolumns c,
			sysobjects o
		WHERE
			o.name like @procedure_name
			AND user_name(o.uid) like @procedure_owner
			AND o.id = c.id
			AND o.type = 'P'						/* Just Procedures */
			AND 'RETURN_VALUE' like @column_name
		ORDER BY procedure_qualifier, procedure_owner, procedure_name, column_type, colid
	end
go

grant execute on sp_sproc_columns to public
go

print "creating sp_table_privileges"
go

CREATE PROCEDURE sp_table_privileges (
			@table_name 		varchar(32),
			@table_owner		varchar(32) = null,
			@table_qualifier	varchar(32) = null)
as

    declare @table_id    int,
			@owner_id	 int,
			@full_table_name char(70)

    if @table_qualifier is not null
    begin
		if db_name() != @table_qualifier
		begin	/* If qualifier doesn't match current database */
			print "Table qualifier must be name of current database"
			return
		end
    end
    if @table_owner is null
	begin	/* If unqualified table name */
		SELECT @full_table_name = @table_name
    end
    else
	begin	/* Qualified table name */
		SELECT @full_table_name = @table_owner + '.' + @table_name
    end
	/*	Get Object ID */
    SELECT @table_id = object_id(@full_table_name)

    if @@trancount != 0
	begin	/* If inside a transaction */
		raiserror 20001 "catalog procedure sp_table_privileges can not be run in a transaction"
		return
    end
	create table #table_priv1(
		table_qualifier		varchar(32),
	    table_owner         varchar(32),
		table_name			varchar(32),
		grantor 			varchar(32),
		grantee 			varchar(32),
	    select_privilege    int,
		insert_privilege	int,
		update_privilege	int,
		delete_privilege	int,
		is_grantable		varchar(3),
		uid 				int,
	    gid                 int)

    insert into #table_priv1
		select distinct
			db_name(),
			user_name(o.uid),
			o.name,
			user_name(o.uid),
			u.name,
			0,
			0,
			0,
			0,
			'no',
			u.uid,
			u.gid
		from sysusers u, sysobjects o
		where o.id = @table_id and u.uid != u.gid
		   and sysstat & 7 in (1,2,3)	/* only valid for system tables,
		   								** user tables, and views. */

    /*
    ** now add row for table owner
    */
    if exists (
		select *
			from #table_priv1
			where grantor = grantee)
    begin
		update #table_priv1
		set
			select_privilege = 1,
			update_privilege = 1,
			insert_privilege = 1,
			delete_privilege = 1,
			is_grantable = 'yes'
		where grantor = grantee
    end
    else
    begin
		insert into #table_priv1
			select	db_name(),
				user_name(o.uid),
				o.name,
				user_name(o.uid),
				user_name(o.uid),
				1,
				1,
				1,
				1,
				'yes',
				o.uid,
				u.gid
			from sysobjects o, sysusers u
			where o.id = @table_id and u.uid = o.uid
			and sysstat & 7 in (1,2,3)	/* only valid for system tables,
		   								** user tables, and views. */

    end

    update #table_priv1
	set select_privilege = 1
	where
		exists (
			select * from sysprotects
			where
				id = @table_id
				and (#table_priv1.uid = uid
					or #table_priv1.gid = uid
					or uid = 0)
				and protecttype = 205
				and action = 193)
		and not exists (
			select * from sysprotects
			where
				id = @table_id
				and (#table_priv1.uid = uid
					or #table_priv1.gid = uid
					or uid = 0)
				and protecttype = 206
				and action = 193)

    update #table_priv1
	set insert_privilege = 1
	where
		exists (
			select * from sysprotects
			where
				id = @table_id
				and (#table_priv1.uid = uid
					or #table_priv1.gid = uid
					or uid = 0)
				and protecttype = 205
				and action = 195)
		and not exists (
			select * from sysprotects
			where
				id = @table_id
				and (#table_priv1.uid = uid
					or #table_priv1.gid = uid
					or uid = 0)
				and protecttype = 206
				and action = 195)

    update #table_priv1
	set delete_privilege = 1
	where
		exists (
			select * from sysprotects
			where
				id = @table_id
				and (#table_priv1.uid = uid
					or #table_priv1.gid = uid
					or uid = 0)
				and protecttype = 205
				and action = 196)
		and not exists (select * from sysprotects
			where
				id = @table_id
				and (#table_priv1.uid = uid
					or #table_priv1.gid = uid
					or uid = 0)
				and protecttype = 206
				and action = 196)

    update #table_priv1
	set update_privilege = 1
	where
		exists (
			select * from sysprotects
			where
				id = @table_id
				and (#table_priv1.uid = uid
					or #table_priv1.gid = uid
					or uid = 0)
				and protecttype = 205
				and action = 197)
		and not exists (
			select * from sysprotects
			where
				id = @table_id
				and (#table_priv1.uid = uid
					or #table_priv1.gid = uid
					or uid = 0)
				and protecttype = 206
				and action = 197)

	create table #table_priv2(
		table_qualifier varchar(32),
		table_owner 	varchar(32),
		table_name		varchar(32),
		grantor 		varchar(32),
		grantee 		varchar(32),
		privilege		varchar(32),
		is_grantable	varchar(3))

	insert into #table_priv2
		select
			table_qualifier,
			table_owner,
			table_name,
			grantor,
			grantee,
			'SELECT',
			is_grantable
		from #table_priv1
		where select_privilege = 1


	insert into #table_priv2
		select
			table_qualifier,
			table_owner,
			table_name,
			grantor,
			grantee,
			'INSERT',
			is_grantable
		from #table_priv1
		where insert_privilege = 1


	insert into #table_priv2
		select
			table_qualifier,
			table_owner,
			table_name,
			grantor,
			grantee,
			'DELETE',
			is_grantable
		from #table_priv1
		where delete_privilege = 1


	insert into #table_priv2
		select
			table_qualifier,
			table_owner,
			table_name,
			grantor,
			grantee,
			'UPDATE',
			is_grantable
		from #table_priv1
		where update_privilege = 1


    select * from #table_priv2
	order by privilege
go

grant execute on sp_table_privileges to public
go

dump tran master with truncate_only
go

print "creating sp_column_privileges"
go


CREATE PROCEDURE sp_column_privileges (
			@table_name 		varchar(32),
			@table_owner		varchar(32) = null,
			@table_qualifier	varchar(32) = null,
			@column_name		varchar(32) = null)
as

    declare @table_id    int,
	@owner_id    int
    DECLARE @full_table_name    char(70)
    declare @low int                    /* range of userids to check */
    declare @high int
    declare @objid int                  /* id of @name if object */
    declare @owner_name varchar(32)

    select @low = 0, @high = 32767

	if @column_name is null /*	If column name not supplied, match all */
		select @column_name = '%'

    if @table_qualifier is not null
    begin
		if db_name() != @table_qualifier
		begin	/* If qualifier doesn't match current database */
			print "Table qualifier must be name of current database"
			return
		end
    end
    if @table_owner is null
	begin	/* If unqualified table name */
		SELECT @full_table_name = @table_name
    end
    else
	begin	/* Qualified table name */
		SELECT @full_table_name = @table_owner + '.' + @table_name
    end
	/*	Get Object ID */
    select @table_id = object_id(@full_table_name)

    if @@trancount != 0
	begin	/* If inside a transaction */
		raiserror 20001 "catalog procedure sp_column_privileges can not be run in a transaction"
		return
    end

    /*
    ** We need to create a table which will contain a row for every row to
    ** be returned to the client.
    */

	create table #column_priv1(
		table_qualifier			varchar(32),
		table_owner 			varchar(32),
		table_name				varchar(32),
		column_name 			varchar(32),
		grantor 				varchar(32),
		grantee 				varchar(32),
		select_privilege		int,
		select_grantable		int,
		insert_privilege		int,
		insert_grantable		int,
		update_privilege		int,
		update_grantable		int,
		delete_privilege		int,
		delete_grantable		int,
	    references_privilege    int null,
	    references_grantable    int null,
		uid 					int,
		gid 					int,
		is_grantable			varchar(3))

/*
** insert a row for the table owner (who has all permissions)
*/
	select @owner_name = (
		select user_name(uid)
		from sysobjects
		where id = @table_id)

    insert into #column_priv1
		select
			db_name(),
			@owner_name,
			@table_name,
			name,
			@owner_name,
			@owner_name,
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			null,
			null,
			user_id(@owner_name),
			0,
			'yes'
		from syscolumns
		where id = @table_id

/*
** now stick a row in the table for every user in the database
** we will need to weed out those who have no permissions later
** (and yes this is a cartesion product: the uid field in sysprotects
** can also have a group id, in which case we need to extend those
** privileges to all group members).
*/

    insert into #column_priv1
		select distinct
			db_name(),
			user_name(o.uid),
			@table_name,
			c.name,
			user_name(o.uid),
			u.name,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			null,
			null,
			u.uid,
			u.gid,
			'no'
		from sysusers u, syscolumns c, sysobjects o
		where o.id = @table_id
			and c.id = o.id
			and u.gid != u.uid
			and u.name != @owner_name

    /*
    ** we need to create another temporary table to contain all the various
    ** protection information for the table in question
    */
	create table #protects (
				uid 		smallint,
				action		tinyint,
			    protecttype tinyint,
				name		varchar(32))

    insert into #protects
		select
			p.uid,
			p.action,
			p.protecttype,
			isnull(col_name(id, c.number), "All")
			from
				sysprotects p,
				master.dbo.spt_values c,
				master.dbo.spt_values a,
				master.dbo.spt_values b
			where
				convert(tinyint, substring(isnull(p.columns, 0x1), c.low, 1))
					& c.high != 0
					and c.number <= (
						select count(*)
						from syscolumns
						where id = @table_id)
				and a.type = "T"
				and a.number = p.action
				and b.type = "T"
				and b.number = p.protecttype
				and p.id = @table_id
				and p.uid between @low and @high


    update #column_priv1
	set select_privilege = 1
	where
		exists (
			select * from #protects
			where
				protecttype = 205
				and action = 193
				and (name = #column_priv1.column_name
					or name = 'All')
				and (uid = 0
					or uid = #column_priv1.gid
					or uid = #column_priv1.uid))
		and not exists (
			select * from #protects
			where
				protecttype = 206
				and action = 193
				and (name = #column_priv1.column_name
					or name = 'All')
				and ( uid = 0
					or uid = #column_priv1.gid
					or uid = #column_priv1.uid))

    update #column_priv1
	set insert_privilege = 1
	where
		exists (
			select * from #protects
			where
				protecttype = 205
				and action = 195
				and (name = #column_priv1.column_name
					or name = 'All')
				and (uid = 0
					or uid = #column_priv1.gid
					or uid = #column_priv1.uid))
		and not exists (
			select * from #protects
			where
				protecttype = 206
				and action = 195
				and (name = #column_priv1.column_name
					or name = 'All')
				and (uid = 0
					or uid = #column_priv1.gid
					or uid = #column_priv1.uid))

    update #column_priv1
	set insert_privilege = 1
	where
		exists (
			select * from #protects
			where
				protecttype = 205
				and action = 196
				and (name = #column_priv1.column_name
					or name = 'All')
				and (uid = 0
					or uid = #column_priv1.gid
					or uid = #column_priv1.uid))
		and not exists (
			select * from #protects
			where
				protecttype = 206
				and action = 196
				and (name = #column_priv1.column_name
					or name = 'All')
				and (uid = 0
					or uid = #column_priv1.gid
					or uid = #column_priv1.uid))


    update #column_priv1
	set update_privilege = 1
		where
			exists (
				select * from #protects
				where protecttype = 205
				and action = 197
				and (name = #column_priv1.column_name
					or name = 'All')
				and (uid = 0
					or uid = #column_priv1.gid
					or uid = #column_priv1.uid))
			and not exists (
				select * from #protects
					where protecttype = 206
					and action = 197
					and (name = #column_priv1.column_name
						or name = 'All')
					and (uid = 0
						or uid = #column_priv1.gid
						or uid = #column_priv1.uid))

	create table #column_priv2(
		table_qualifier varchar(32),
		table_owner 	varchar(32),
		table_name		varchar(32),
		column_name 	varchar(32),
		grantor 		varchar(32),
		grantee 		varchar(32),
		privilege		varchar(32),
		is_grantable	varchar(3))

	insert into #column_priv2
		select
			table_qualifier,
			table_owner,
			table_name,
			column_name,
			grantor,
			grantee,
			'SELECT',
			is_grantable
		from #column_priv1
		where select_privilege = 1

	insert into #column_priv2
		select
			table_qualifier,
			table_owner,
			table_name,
			column_name,
			grantor,
			grantee,
			'INSERT',
			is_grantable
		from #column_priv1
		where insert_privilege = 1

	insert into #column_priv2
		select
			table_qualifier,
			table_owner,
			table_name,
			column_name,
			grantor,
			grantee,
			'UPDATE',
			is_grantable
		from #column_priv1
		where update_privilege = 1

	select * from #column_priv2
	where column_name like @column_name
	order by column_name, privilege
go

grant execute on sp_column_privileges to public
go

dump tran master with truncate_only
go


print "creating sp_server_info"
go

create proc sp_server_info (
			@attribute_id  int = null)
as
    if @attribute_id is not null
		select *
		from master.dbo.spt_server_info
		where attribute_id = @attribute_id
    else
		select *
		from master.dbo.spt_server_info
		order by attribute_id
go

grant execute on sp_server_info to public
go

print "creating sp_datatype_info"
go

/* the messiness of 'data_type' was to get around the problem of
returning the correct lengths for user defined types.  the join
on the type name ensures all user defined types are returned, but
this puts a null in the data_type column.  by forcing an embedded
select and correlating it with the current row in systypes, we get
the correct data_type mapping even for user defined types  (kwg) */

create proc sp_datatype_info
	(@data_type int = 0)
as
	if @data_type = 0
	   		select	/* Real SQL Server data types */
			type_name = t.name,
			d.data_type,
			precision = isnull(d.data_precision, convert(int,t.length)),
			d.literal_prefix,
			d.literal_suffix,
			e.create_params,
			d.nullable,
			d.case_sensitive,
			d.searchable,
			d.unsigned_attribute,
			d.money,
			d.auto_increment,
			d.local_type_name,
			minimum_scale = d.numeric_scale,
			maximum_scale = d.numeric_scale
		from master.dbo.spt_datatype_info d, master.dbo.spt_datatype_info_ext e, systypes t
		where
			d.ss_dtype = t.type
			and t.usertype *= e.user_type
			and t.usertype not in (80,18)		/* No SYSNAME or TIMESTAMP */
			and t.usertype < 100				/* No user defined types */
			and t.type not in (111,109,38,110)	/* get rid of nullable types */
			and t.name != 'nchar'				/* get rid of 4.9.1 type */
			and t.name != 'nvarchar'			/* get rid of 4.9.1 type */
		UNION
		select	/* SQL Server SYSNAME, TIMESTAMP and user data types */
			type_name = t.name,
			d.data_type,
			precision = isnull(d.data_precision, convert(int,t.length)),
			d.literal_prefix,
			d.literal_suffix,
			e.create_params,
			d.nullable,
			d.case_sensitive,
			d.searchable,
			d.unsigned_attribute,
			d.money,
			d.auto_increment,
			t.name,
			minimum_scale = d.numeric_scale,
			maximum_scale = d.numeric_scale
		from master.dbo.spt_datatype_info d, master.dbo.spt_datatype_info_ext e, systypes t
		where
			d.ss_dtype = t.type
			and t.usertype *= e.user_type
			and (t.usertype in (80,18)			/* SYSNAME or TIMESTAMP */
				or t.usertype >= 100)			/* User defined types */
			and t.type not in (111,109,38,110)	/* get rid of nullable types */
		order by d.data_type, type_name

	else
		select	/* Real SQL Server data types */
			type_name = t.name,
			d.data_type,
			precision = isnull(d.data_precision, convert(int,t.length)),
			d.literal_prefix,
			d.literal_suffix,
			e.create_params,
			d.nullable,
			d.case_sensitive,
			d.searchable,
			d.unsigned_attribute,
			d.money,
			d.auto_increment,
			d.local_type_name,
			minimum_scale = d.numeric_scale,
			maximum_scale = d.numeric_scale
		from master.dbo.spt_datatype_info d, master.dbo.spt_datatype_info_ext e, systypes t
		where
			data_type = @data_type
			and d.ss_dtype = t.type
			and t.usertype *= e.user_type
			and t.usertype not in (80,18)		/* No SYSNAME or TIMESTAMP */
			and t.usertype < 100				/* No user defined types */
			and t.type not in (111,109,38,110)	/* get rid of nullable types */
			and t.name != 'nchar'				/* get rid of 4.9.1 type */
			and t.name != 'nvarchar'			/* get rid of 4.9.1 type */
		UNION
		select	/* SQL Server SYSNAME, TIMESTAMP and user data types */
			type_name = t.name,
			d.data_type,
			precision = isnull(d.data_precision, convert(int,t.length)),
			d.literal_prefix,
			d.literal_suffix,
			e.create_params,
			d.nullable,
			d.case_sensitive,
			d.searchable,
			d.unsigned_attribute,
			d.money,
			d.auto_increment,
			t.name,
			minimum_scale = d.numeric_scale,
			maximum_scale = d.numeric_scale
		from master.dbo.spt_datatype_info d, master.dbo.spt_datatype_info_ext e, systypes t
		where
			data_type = @data_type
			and d.ss_dtype = t.type
			and t.usertype *= e.user_type
			and (t.usertype in (80,18)			/* SYSNAME or TIMESTAMP */
				or t.usertype >= 100)			/* User defined types */
			and t.type not in (111,109,38,110)	/* get rid of nullable types */
		order by type_name

go

grant execute on sp_datatype_info to public
go

dump tran master with truncate_only
go

print "creating sp_special_columns"
go

dump tran master with truncate_only
go

CREATE PROCEDURE sp_special_columns (
				 @table_name		varchar(32),
				 @table_owner		varchar(32) = null,
				 @table_qualifier	varchar(32) = null,
				 @col_type			char(1) = 'R',
				 @scope				char(1) = 'T',
				 @nullable			char(1) = 'U')
AS
	DECLARE @indid				int
	DECLARE @table_id			int
    DECLARE @full_table_name    char(70)
	DECLARE @msg				char(70)
	DECLARE @scopeout			smallint

	if @col_type not in ('R','V')
	   begin
	      print "Invalid value - please enter either 'R' or 'V' for col_type."
		  return
	   end

	if @scope = 'C'
		select @scopeout = 0
	else if @scope = 'T'
		select @scopeout = 1
	else
	   begin
	      print "Invalid value - please enter either 'C' or 'T' for scope."
		  return
	   end

	if @nullable not in ('U','O')
	   begin
	      print "Invalid value - please enter either 'U' or 'O' for nullable."
		  return
	   end

    if @table_qualifier is not null
	   begin
		  if db_name() != @table_qualifier
		      begin	/* If qualifier doesn't match current database */
			    print "Table qualifier must be name of current database"
			    return
		      end
	   end
    if @table_owner is null
	   begin	/* If unqualified table name */
		  SELECT @full_table_name = @table_name
       end
    else
	   begin	/* Qualified table name */
		  SELECT @full_table_name = @table_owner + '.' + @table_name
       end
	/*	Get Object ID */
	SELECT @table_id = object_id(@full_table_name)

    if @col_type = 'V'
	BEGIN /* if ROWVER, just run that query */
		SELECT
			scope = convert(smallint,NULL),
			column_name = c.name,
			data_type = d.data_type+convert(smallint,
							isnull(d.aux,
							ascii(substring("666AAA@@@CB??GG",
							2*(d.ss_dtype%35+1)+2-8/c.length,1))
							-60)),
			type_name = t.name,
			precision = isnull(d.data_precision, convert(int,c.length))
							+isnull(d.aux, convert(int,
							ascii(substring("???AAAFFFCKFOLS",
							2*(d.ss_dtype%35+1)+2-8/c.length,1))-60)),
			length = isnull(d.length, convert(int,c.length)) +convert(int,
							isnull(d.aux,
							ascii(substring("AAA<BB<DDDHJSPP",
							2*(d.ss_dtype%35+1)+2-8/c.length,
							1))-64)),
			scale = d.numeric_scale +convert(smallint,
							isnull(d.aux,
							ascii(substring("<<<<<<<<<<<<<<?",
							2*(d.ss_dtype%35+1)+2-8/c.length,
							1))-60)),
			pseudo_column = convert(smallint,1)
		FROM
			systypes t, syscolumns c, master.dbo.spt_datatype_info d
		WHERE
			c.id = @table_id
			AND c.type = d.ss_dtype
			AND c.usertype = 80 /*	TIMESTAMP */
			AND t.usertype = 80 /*	TIMESTAMP */
		RETURN
	END

	/* ROWID, now find the id of the 'best' index for this table */

	IF @nullable = 'O'	/* Don't include any indexes that contain
						   nullable columns. */

			SELECT @indid = MIN(indid)
				FROM sysindexes i,syscolumns c,syscolumns c2
				WHERE
					i.status&2 = 2		/*	If Unique Index */
		 			AND c.id = i.id
		 			AND c2.id = c.id
		 			AND c2.colid < i.keycnt + (i.status&16)/16
					AND i.id = @table_id
					AND indid > 0		/*	Eliminate Table Row */
					AND c.name = index_col(@table_name,i.indid,c2.colid)
					GROUP BY indid HAVING SUM(c.status&8) = 0

	ELSE	/* Include indexes that are partially nullable. */

		SELECT @indid = MIN(indid)
			FROM sysindexes i
			WHERE
				status&2 = 2		/*	If Unique Index */
				AND id = @table_id
				AND indid > 0		/*	Eliminate Table Row */

	SELECT
		scope = @scopeout,
		column_name = INDEX_COL(@full_table_name,indid,c.colid),
		data_type = d.data_type+convert(smallint,
						isnull(d.aux,
						ascii(substring("666AAA@@@CB??GG",
						2*(d.ss_dtype%35+1)+2-8/c2.length,1))
						-60)),
		type_name = rtrim(substring(d.type_name,
						1+isnull(d.aux,
						ascii(substring("III<<<MMMI<<A<A",
						2*(d.ss_dtype%35+1)+2-8/c2.length,
						1))-60), 13)),
		precision = isnull(d.data_precision, convert(int,c2.length))
						+isnull(d.aux, convert(int,
						ascii(substring("???AAAFFFCKFOLS",
						2*(d.ss_dtype%35+1)+2-8/c2.length,1))-60)),
		length = isnull(d.length, convert(int,c2.length)) +convert(int,
						isnull(d.aux,
						ascii(substring("AAA<BB<DDDHJSPP",
						2*(d.ss_dtype%35+1)+2-8/c2.length,
						1))-64)),
		scale = d.numeric_scale +convert(smallint,
						isnull(d.aux,
						ascii(substring("<<<<<<<<<<<<<<?",
						2*(d.ss_dtype%35+1)+2-8/c2.length,
						1))-60)),
		pseudo_column = convert(smallint,1)
	FROM
		sysindexes x,
		syscolumns c,
		master.dbo.spt_datatype_info d,
		systypes t,
		syscolumns c2	/* Self-join to generate list of index columns and */
						/* to extract datatype names */
	WHERE
		x.id = @table_id
		AND c2.name = INDEX_COL(@full_table_name,@indid,c.colid)
		AND c2.id =x.id
		AND c.id = x.id
		AND c.colid < keycnt+(x.status&16)/16
		AND x.indid = @indid
		AND c2.type = d.ss_dtype
		AND c2.usertype *= t.usertype

go

grant execute on sp_special_columns to public
go

print "creating sp_databases"
go

create proc sp_databases
as
	/* Use temporary table to sum up database size w/o using group by */
	create table #databases (
				  database_name varchar(32),
				  size int)

	/* Insert row for each database */
	insert into #databases
		select
			name,
			(select sum(size) from master.dbo.sysusages
				where dbid = d.dbid)
		from master.dbo.sysdatabases d

	select
		 database_name,
		 database_size = size*2,	/* Convert from 2048 byte pages to K */
		 remarks = convert(varchar(254),null)	/* Remarks are NULL */
	from #databases
	order by database_name
go

grant execute on sp_databases to public
go

dump tran master with truncate_only
go

/*******************************************************************************/
/* This portion returns everything back to normal                   */
/*******************************************************************************/
use master
go

if exists (select * from sysobjects
			where name = 'sp_configure' and sysstat & 7 = 4)
begin
    execute sp_configure 'update',0
end
reconfigure with override
go

checkpoint
go
