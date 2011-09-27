
/*
**	INSTCAT.SQL
**	Installs catalog stored procedures on the Microsoft SQL Server.
**	Copyright 1992-1996, Microsoft Corp.	All rights reserved.
*/

/*
NOTE:  you MUST change the last row inserted into spt_server_info
to be version number of this file.	the convention is j.nn.bbb, where
jj is the major version number ('6' now), nn is the minor version number
('50' now), and bbb is the build number.
*/

/****************************************************************************/
/* This portion sets up the ability to perform all the functions in this    */
/* script                                                                    */
/****************************************************************************/
go
use master
go
dump tran master with no_log
go

set quoted_identifier on
go

if exists (select * from sysobjects
	   where name = 'sp_configure' and sysstat & 0xf = 4)
begin
    execute sp_configure 'update',1
end
reconfigure with override
go

exec sp_MS_upd_sysobj_category 1 --Capture datetime for use below.

go

/*
** If old versions of tables exist, drop them.
*/
if (exists (select * from sysobjects
		where name = 'MSdatatype_info' and sysstat & 0xf = 3))
    drop table MSdatatype_info
go
if (exists (select * from sysobjects
		where name = 'MSdatatype_info_ext' and sysstat & 0xf = 3))
    drop table MSdatatype_info_ext
go
if (exists (select * from sysobjects
		where name = 'MStable_types' and sysstat & 0xf = 3))
    drop table MStable_types
go
if (exists (select * from sysobjects
		where name = 'MSserver_info' and sysstat & 0xf = 3))
    drop table MSserver_info
go
if (exists (select * from sysobjects
		where name = 'spt_table_types' and sysstat & 0xf = 3))
	drop table spt_table_types	/* no longer used */
go

/*
** If tables or procs already exist, drop them.
*/

if (exists (select * from sysobjects
		where name = 'spt_datatype_info' and sysstat & 0xf = 3))
    drop table spt_datatype_info
go
if (exists (select * from sysobjects
		where name = 'spt_datatype_info_ext' and sysstat & 0xf = 3))
    drop table spt_datatype_info_ext
go
if (exists (select * from sysobjects
		where name = 'spt_server_info' and sysstat & 0xf = 3))
    drop table spt_server_info
go
if (exists (select * from sysobjects
		where name = 'sp_tables' and sysstat & 0xf = 4))
    drop proc sp_tables
go
if (exists (select * from sysobjects
		where name = 'sp_statistics' and sysstat & 0xf = 4))
    drop proc sp_statistics
go
if (exists (select * from sysobjects
		where name = 'sp_columns' and sysstat & 0xf = 4))
    drop proc sp_columns
go
if (exists (select * from sysobjects
		where name = 'sp_fkeys' and sysstat & 0xf = 4))
    drop proc sp_fkeys
go
if (exists (select * from sysobjects
		where name = 'sp_pkeys' and sysstat & 0xf = 4))
    drop proc sp_pkeys
dump tran master with no_log
go

go
if (exists (select * from sysobjects
		where name = 'sp_stored_procedures' and sysstat & 0xf = 4))
    drop proc sp_stored_procedures
go
if (exists (select * from sysobjects
		where name = 'sp_sproc_columns' and sysstat & 0xf = 4))
    drop proc sp_sproc_columns
go
if (exists (select * from sysobjects
		where name = 'sp_table_privileges' and sysstat & 0xf = 4))
    drop proc sp_table_privileges
go
if (exists (select * from sysobjects
		where name = 'sp_column_privileges' and sysstat & 0xf = 4))
	drop proc sp_column_privileges
go
if (exists (select * from sysobjects
		where name = 'sp_server_info' and sysstat & 0xf = 4))
    drop proc sp_server_info
go
if (exists (select * from sysobjects
		where name = 'sp_datatype_info' and sysstat & 0xf = 4))
    drop proc sp_datatype_info
go
if (exists (select * from sysobjects
		where name = 'sp_special_columns' and sysstat & 0xf = 4))
    drop proc sp_special_columns
go
if (exists (select * from sysobjects
		where name = 'sp_databases' and sysstat & 0xf = 4))
	drop proc sp_databases
go
if (exists (select * from sysobjects
		where name = 'sp_ddopen' and sysstat & 0xf = 4))
	drop proc sp_ddopen
go

dump tran master with no_log
go

print 'creating table spt_datatype_info_ext'
go
create table spt_datatype_info_ext (
				user_type		smallint	not null,
				CREATE_PARAMS	varchar(32) null,
				AUTO_INCREMENT	smallint null)
go

create unique clustered index datatypeinfoextclust on spt_datatype_info_ext(user_type,AUTO_INCREMENT)
go

grant select on spt_datatype_info_ext to public
go


insert into spt_datatype_info_ext
	/* CHAR 	 user_type, create_params, auto_increment */
	values			 (1,	'length' ,0)

insert into spt_datatype_info_ext
	/* VARCHAR	 user_type, create_params, auto_increment */
	values			 (2,	'max length' ,0)

insert into spt_datatype_info_ext
	/* BINARY	 user_type, create_params, auto_increment */
	values			 (3,	'length' ,0)

insert into spt_datatype_info_ext
	/* VARBINARY user_type, create_params, auto_increment */
	values			 (4,	'max length' ,0)

if	(charindex('6.00', @@version) > 0 or
	 charindex('6.50', @@version) > 0 or
	 charindex('7.00', @@version) > 0)
begin	/*	Add 6.0 data types */
	insert into spt_datatype_info_ext
		/* DECIMAL user_type, create_params, auto_increment */
		values			 (26,	'precision,scale' ,0)

	insert into spt_datatype_info_ext
		/* NUMERIC user_type, create_params, auto_increment */
		values			 (25,	'precision,scale' ,0)

	insert into spt_datatype_info_ext
		/* DECIMAL IDENTITY user_type, create_params, auto_increment */
		values			 (26,	'precision' ,1)

	insert into spt_datatype_info_ext
		/* NUMERIC IDENTITY user_type, create_params, auto_increment */
		values			 (25,	'precision' ,1)

end
else	/*	Pre 6.0 server, add SYSNAME create param */
	begin
		insert into spt_datatype_info_ext
			/* SYSNAME	 user_type, create_param, auto_increments */
			values			 (18,	'max length' ,0)

	end
go

print 'creating table spt_datatype_info'
go
create table spt_datatype_info (
	ss_dtype			tinyint 	not null,
	fixlen				int 		null,		/* datatype len for variable, else null */
	ODBCVer 			tinyint 	null,		/* version if needed, else null */
	TYPE_NAME			varchar(30)	not null,
	DATA_TYPE			smallint	not null,
	data_precision		int 		null,
	numeric_scale		smallint	null,	/* min scale if 6.0 */
	RADIX				smallint	null,
	length				int 		null,
	LITERAL_PREFIX		varchar(32) null,
	LITERAL_SUFFIX		varchar(32) null,
	CREATE_PARAMS		varchar(32) null,
	NULLABLE			smallint	not null,
	CASE_SENSITIVE		smallint	not null,
	SEARCHABLE			smallint	not null,
	UNSIGNED_ATTRIBUTE	smallint	null,
	MONEY				smallint	not null,
	AUTO_INCREMENT		smallint	null,
	LOCAL_TYPE_NAME 	varchar(128)null,
	charbin 			tinyint 	null, /* 0 for char/binary types, NULL for all others */
	SQL_DATA_TYPE		smallint	not null,
	SQL_DATETIME_SUB	smallint	null)
go

create unique clustered index datatypeinfoclust on spt_datatype_info(ss_dtype,fixlen,ODBCVer,AUTO_INCREMENT)
go

grant select on spt_datatype_info to public
go

/* Get case sensitivity */
if 'A' = 'A' /* create dummy begin block */
begin
	declare @case smallint

	begin tran
	select @case = 0
	select @case = 1 where 'a' <> 'A'

	/* Local Binary */
	insert into spt_datatype_info values
	(45,null,null,'binary',-2,null,null,null,null,'0x',null,'length',1,0,2,null,0,null,'binary',0,-2,null)

	/* Local Bit */
	insert into spt_datatype_info values
	(50,null,null,'bit',-7,1,0,2,null,null,null,null,0,0,2,null,0,null,'bit',null,-7,null)

	/* Local Char */
	insert into spt_datatype_info values
	(47,null,null,'char',1,null,null,null,null,'''','''','length',1,@case,3,null,0,null,'char',0,1,null)

	/* Local Datetime */
	insert into spt_datatype_info values
	(61,8,2,'datetime',11,23,3,10,16,'''','''',null,1,0,3,null,0,null,'datetime',null,9,3)
	insert into spt_datatype_info values
	(61,8,3,'datetime',93,23,3,10,16,'''','''',null,1,0,3,null,0,null,'datetime',null,9,3)

	/* Local Smalldatetime */
	insert into spt_datatype_info values
	(58,4,2,'smalldatetime',11,16,0,10,16,'''','''',null,1,0,3,null,0,null,'smalldatetime',null,9,3)
	insert into spt_datatype_info values
	(58,4,3,'smalldatetime',93,16,0,10,16,'''','''',null,1,0,3,null,0,null,'smalldatetime',null,9,3)

	/* Local Datetimn */
	insert into spt_datatype_info values
	(111,4,2,'smalldatetime',11,16,0,10,16,'''','''',null,1,0,3,null,0,null,'smalldatetime',null,9,3)
	insert into spt_datatype_info values
	(111,4,3,'smalldatetime',93,16,0,10,16,'''','''',null,1,0,3,null,0,null,'smalldatetime',null,9,3)
	insert into spt_datatype_info values /* sql server type is 'datetimn' */
	(111,8,2,'datetime',11,23,3,10,16,'''','''',null,1,0,3,null,0,null,'datetime',null,9,3)
	insert into spt_datatype_info values
	(111,8,3,'datetime',93,23,3,10,16,'''','''',null,1,0,3,null,0,null,'datetime',null,9,3)

	/* Local Float */
	insert into spt_datatype_info values
	(62,8,2,'float',6,15,null,10,8,null,null,null,1,0,2,0,0,0,'float',null,6,null)
	insert into spt_datatype_info values
	(62,8,3,'float',6,53,null, 2,8,null,null,null,1,0,2,0,0,0,'float',null,6,null)

	/* Local Floatn */
	insert into spt_datatype_info values /* sql server type is 'floatn' */
	(109,4,2,'real',7, 7,null,10,4,null,null,null,1,0,2,0,0,0,'real',null,7,null)
	insert into spt_datatype_info values
	(109,4,3,'real',7,24,null, 2,4,null,null,null,1,0,2,0,0,0,'real',null,7,null)
	insert into spt_datatype_info values /* sql server type is 'floatn' */
	(109,8,2,'float',6,15,null,10,8,null,null,null,1,0,2,0,0,0,'float',null,6,null)
	insert into spt_datatype_info values
	(109,8,3,'float',6,53,null, 2,8,null,null,null,1,0,2,0,0,0,'float',null,6,null)

	/* Local Real */
	insert into spt_datatype_info values
	(59,4,2,'real',7, 7,null,10,4,null,null,null,1,0,2,0,0,0,'real',null,7,null)
	insert into spt_datatype_info values
	(59,4,3,'real',7,24,null, 2,4,null,null,null,1,0,2,0,0,0,'real',null,7,null)

	/* Local Smallmoney */
	insert into spt_datatype_info values
	(122,4,null,'smallmoney',3,10,4,10,12,'$',null,null,1,0,2,0,1,0,'smallmoney',null,3,null)

	/* Local Money */
	insert into spt_datatype_info values
	(60,8,null,'money',3,19,4,10,21,'$',null,null,1,0,2,0,1,0,'money',null,3,null)

	/* Local Moneyn */
	insert into spt_datatype_info values	/* sql server type is 'moneyn' */
	(110,4,null,'smallmoney',3,10,4,10,12,'$',null,null,1,0,2,0,1,0,'smallmoney',null,3,null)
	insert into spt_datatype_info values	/* sql server type is 'moneyn' */
	(110,8,null,'money',3,19,4,10,21,'$',null,null,1,0,2,0,1,0,'money',null,3,null)

	/* Local Int */
	insert into spt_datatype_info values
	(56,4,null,'int',4,10,0,10,4,null,null,null,1,0,2,0,0,0,'int',null,4,null)

	/* Local Intn */
	insert into spt_datatype_info values /* sql server type is 'intn' */
	(38,4,null,'int',4,10,0,10,4,null,null,null,1,0,2,0,0,0,'int',null,4,null)
	insert into spt_datatype_info values /* sql server type is 'intn' */
	(38,2,null,'smallint',5,5,0,10,2,null,null,null,1,0,2,0,0,0,'smallint',null,5,null)
	insert into spt_datatype_info values
	(38,1,null,'tinyint',-6,3,0,10,1,null,null,null,1,0,2,1,0,0,'tinyint',null,-6,null)

	/* Local Smallint */
	insert into spt_datatype_info values
	(52,2,null,'smallint',5,5,0,10,2,null,null,null,1,0,2,0,0,0,'smallint',null,5,null)
	insert into spt_datatype_info values
	(52,2,1,'smallint',5,5,0,10,2,null,null,null,1,0,2,0,0,0,'smallint',null,5,null)

	/* Local Tinyint */
	insert into spt_datatype_info values
	(48,1,null,'tinyint',-6,3,0,10,1,null,null,null,1,0,2,1,0,0,'tinyint',null,-6,null)

	/* Local Text */
	insert into spt_datatype_info values
	(35,null,null,'text',-1,2147483647,null,null,2147483647,'''','''',null,1,@case,1,null,0,null,'text',0,-1,null)

	/* Local Varbinary */
	insert into spt_datatype_info values
	(37,null,null,'varbinary',-3,null,null,null,null,'0x',null,'max length',1,0,2,null,0,null,'varbinary',0,-3,null)

	/* Local Varchar */
	insert into spt_datatype_info values
	(39,null,null,'varchar',12,null,null,null,null,'''','''','max length',1,@case,3,null,0,null,'varchar',0,12,null)

	/* Local Image */
	insert into spt_datatype_info values
	(34,null,null,'image',-4,2147483647,null,null,2147483647,'0x',null,null,1,0,0,null,0,null,'image',0,-4,null)

	if (charindex('6.00', @@version) > 0 or
		charindex('6.50', @@version) > 0 or
		charindex('7.00', @@version) > 0)
	begin	/*	Add 6.0 data types */
		/* Local Decimaln */
		insert into spt_datatype_info values	/* sql server type is 'decimaln' */
		(106,null,null,'decimal',3,38,0,10,null,null,null,'precision,scale',1,0,2,0,0,0,'decimal',null,3,null)

		/* Local Numericn */
		insert into spt_datatype_info values	/* sql server type is 'numericn' */
		(108,null,null,'numeric',2,38,0,10,null,null,null,'precision,scale',1,0,2,0,0,0,'numeric',null,2,null)

		/* Local Decimal */
		insert into spt_datatype_info values	/* sql server type is 'decimaln' */
		(55,null,null,'decimal',3,38,0,10,null,null,null,'precision,scale',1,0,2,0,0,0,'decimal',null,3,null)

		/* Local Numeric */
		insert into spt_datatype_info values	/* sql server type is 'numericn' */
		(63,null,null,'numeric',2	,38,0,10,null,null,null,'precision,scale',1,0,2,0,0,0,'numeric',null,2,null)

		/* Identity attribute data types */

		/* Identity Int */
		insert into spt_datatype_info values
		(56,null,null,'int identity',4,10,0,10,null,null,null,null,0,0,2,0,0,1,'int identity',null,4,null)

		/* Identity Smallint */
		insert into spt_datatype_info values
		(52,null,null,'smallint identity',5,5,0,10,null,null,null,null,0,0,2,0,0,1,'smallint identity',null,5,null)

		/* Identity Tinyint */
		insert into spt_datatype_info values
		(48,null,null,'tinyint identity',-6,3,0,10,null,null,null,null,0,0,2,1,0,1,'tinyint identity',null,-6,null)

		/* Identity Numeric */
		insert into spt_datatype_info values	/* sql server type is 'decmaln' */
		(106,null,null,'decimal() identity',3,38,0,10,null,null,null,'precision,scale',0,0,2,0,0,1,'decimal() identity',null,3,null)
		insert into spt_datatype_info values	/* sql server type is 'decmaln' */
		(55,null,null,'decimal() identity',3,38,0,10,null,null,null,'precision,scale',0,0,2,0,0,1,'decimal() identity',null,3,null)

		/* Identity Numeric */
		insert into spt_datatype_info values	/* sql server type is 'decmaln' */
		(108,null,null,'numeric() identity',2,38,0,10,null,null,null,'precision,scale',0,0,2,0,0,1,'numeric() identity',null,2,null)
		insert into spt_datatype_info values	/* sql server type is 'decmaln' */
		(63,null,null,'numeric() identity',2,38,0,10,null,null,null,'precision,scale',0,0,2,0,0,1,'numeric() identity',null,2,null)

	end
	commit tran
end
go

dump tran master with no_log
go

print 'creating table spt_server_info'
go
create table spt_server_info (
			  attribute_id		int NOT NULL,
			  attribute_name	varchar(60) NOT NULL,
			  attribute_value	varchar(255) NOT NULL)
go

create unique clustered index serverinfoclust on spt_server_info(attribute_id)
go

insert into spt_server_info
	values (1,'DBMS_NAME','Microsoft SQL Server')
insert into spt_server_info
	values (2,'DBMS_VER',@@version)
insert into spt_server_info
	values (10,'OWNER_TERM','owner')
insert into spt_server_info
	values (11,'TABLE_TERM','table')
insert into spt_server_info
	values (12,'MAX_OWNER_NAME_LENGTH','30')
insert into spt_server_info
	values (13,'TABLE_LENGTH','30')
insert into spt_server_info
	values (14,'MAX_QUAL_LENGTH','30')
insert into spt_server_info
	values (15,'COLUMN_LENGTH','30')
if 'A' = 'a' /* If not case sensitive server */
begin
	insert into spt_server_info
		values (16,'IDENTIFIER_CASE','MIXED')
end
else
begin
	insert into spt_server_info
		values (16,'IDENTIFIER_CASE','SENSITIVE')
end
insert into spt_server_info
	values (17,'TX_ISOLATION','2')
if (charindex('6.00', @@version) > 0 or
	charindex('6.50', @@version) > 0 or
	charindex('7.00', @@version) > 0)
begin	/*	Add 6.0 collation sequence */
	insert into spt_server_info
		select 18,'COLLATION_SEQ',
			'charset='+t2.name+' sort_order='+t1.name
			+' charset_num='+rtrim(convert(char(4),t1.csid))+
			' sort_order_num='+rtrim(convert(char(4),t1.id))
		from syscharsets t1, syscharsets t2, sysconfigures t3
		where t1.csid=t2.id and t1.id=t3.value and t3.config=1123
end
else
begin	/*	Add 4.2x collation sequence */
	insert into spt_server_info
		select 18,'COLLATION_SEQ',
			'charset='+t2.name+' sort_order='+t1.name
			+' charset_num='+rtrim(convert(char(4),t1.csid))+
			' sort_order_num='+rtrim(convert(char(4),t1.id))
		from syscharsets t1, syscharsets t2, sysconfigures t3
		where t1.csid=t2.id and t1.id=t3.value and t3.config=123
end
insert into spt_server_info
	values (19,'SAVEPOINT_SUPPORT','Y')
insert into spt_server_info
	values (20,'MULTI_RESULT_SETS','Y')
insert into spt_server_info
	values (22,'ACCESSIBLE_TABLES','Y')
insert into spt_server_info
	values (100,'USERID_LENGTH','30')
insert into spt_server_info
	values (101,'QUALIFIER_TERM','database')
insert into spt_server_info
	values (102,'NAMED_TRANSACTIONS','Y')
insert into spt_server_info
	values (103,'SPROC_AS_LANGUAGE','Y')
insert into spt_server_info
	values (104,'ACCESSIBLE_SPROC','Y')
insert into spt_server_info
	values (105,'MAX_INDEX_COLS','16')
insert into spt_server_info
	values (106,'RENAME_TABLE','Y')
insert into spt_server_info
	values (107,'RENAME_COLUMN','Y')
insert into spt_server_info
	values (108,'DROP_COLUMN','N')
insert into spt_server_info
	values (109,'INCREASE_COLUMN_LENGTH','N')
if (charindex('6.50', @@version) = 0 and
	charindex('7.00', @@version) = 0)
begin
	insert into spt_server_info
		values (110,'DDL_IN_TRANSACTION','N')
end
else
begin
	insert into spt_server_info
		values (110,'DDL_IN_TRANSACTION','Y')
end
insert into spt_server_info
	values (111,'DESCENDING_INDEXES','N')
insert into spt_server_info
	values (112,'SP_RENAME','Y')
insert into spt_server_info
	values (113,'REMOTE_SPROC','Y')
if (charindex('7.00', @@version) = 0)
begin
	insert into spt_server_info
		values (500,'SYS_SPROC_VERSION','6.50.193')
end
else
begin
	insert into spt_server_info
		values (500,'SYS_SPROC_VERSION','7.00.005')
end
go

grant select on spt_server_info to public
go

print 'creating sp_column_privileges'
go

/*	Procedure for pre 6.50 server */
CREATE PROCEDURE sp_column_privileges (
			@table_name 		varchar(32),
			@table_owner		varchar(32) = null,
			@table_qualifier	varchar(32) = null,
			@column_name		varchar(90) = null)
as

	declare @table_id	 int
	DECLARE @full_table_name	char(70)
	declare @low smallint					 /* range of userids to check */
	declare @high smallint
	declare @owner_uid smallint

    select @low = 0, @high = 32767

	if @column_name is null /*	If column name not supplied, match all */
		select @column_name = '%'

	if @table_qualifier is not null
    begin
		if db_name() <> @table_qualifier
		begin	/* If qualifier doesn't match current database */
			raiserror 20001 'Table qualifier must be name of current database'
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

	if (@@trancount <> 0)
	begin	/* If inside a transaction */
		raiserror 20003 'The procedure ''sp_column_privileges'' cannot be executed from within a transaction.'
		return
    end

    /*
    ** We need to create a table which will contain a row for every row to
    ** be returned to the client.
    */

	create table #column_priv1(
		COLUMN_NAME 			varchar(32) NOT NULL,
		grantor 				smallint NOT NULL,
		grantee 				smallint NOT NULL,
		select_privilege		bit,
		select_grantable		bit,
		insert_privilege		bit,
		insert_grantable		bit,
		update_privilege		bit,
		update_grantable		bit,
		references_privilege	bit,
		references_grantable	bit,
		uid 					smallint NOT NULL,
		gid 					smallint NOT NULL)

/*
** insert a row for the table owner (who has all permissions)
*/
	select @owner_uid = (
		select uid
		from sysobjects
		where id = @table_id)

	if (charindex('6.00', @@version) > 0)
	begin
		insert into #column_priv1
			select
				c.name,
				u.uid,
				@owner_uid,
				0,
				1,
				0,
				1,
				0,
				1,
				0,
				1,
				@owner_uid,
				0
			from syscolumns c, sysusers u
			where id = @table_id
				and c.number = 0
				and u.suid = 1		/* grantor is dbo of database */
	end
	else
	begin
		insert into #column_priv1
			select
				c.name,
				u.uid,
				@owner_uid,
				0,
				1,
				0,
				1,
				0,
				1,
				0,
				0,
				@owner_uid,
				0
			from syscolumns c, sysusers u
			where id = @table_id
				and c.number = 0
				and u.suid = 1		/* grantor is dbo of database */
	end
/*
** now stick in a row for every column for every user in the database
** we will need to weed out those who have no permissions later
** (and yes this is a cartesion product: the uid field in sysprotects
** can also have a group id, in which case we need to extend those
** privileges to all group members).
*/

    insert into #column_priv1
		select distinct
			c.name,
			o.uid,
			u.uid,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			u.uid,
			u.gid
		from sysusers u, syscolumns c, sysobjects o
		where o.id = @table_id
			and c.id = o.id
			and c.number = 0
			and u.gid <> u.uid
			and u.uid <> @owner_uid

    /*
    ** we need to create another temporary table to contain all the various
    ** protection information for the table in question
    */
	create table #protects (
				uid 		smallint NOT NULL,
				grantor		smallint NOT NULL,
				action		tinyint NOT NULL,
				protecttype tinyint NOT NULL,
				name		varchar(32) NOT NULL)

    insert into #protects
		select
			p.uid,
			p.uid,
			p.action,
			p.protecttype,
			isnull(col_name(id, c.number), '~All')
			from
				sysprotects p,
				master.dbo.spt_values c,
				master.dbo.spt_values a,
				master.dbo.spt_values b
			where
				convert(tinyint, substring(isnull(p.columns, 0x1), c.low, 1))
					& c.high <> 0
					and c.number <= (
						select count(*)
						from syscolumns
						where id = @table_id)
				and c.type = 'P'
				and a.type = 'T'
				and a.number = p.action
				and p.action in (193,195,197,26)
				and b.type = 'T'
				and b.number = p.protecttype
				and p.id = @table_id
				and p.uid between @low and @high


    update #column_priv1
	set select_privilege = 1
		from #protects p
	where
		p.protecttype = 205
		and p.action = 193
		and (p.name = #column_priv1.COLUMN_NAME
			or name = '~All')
		and (p.uid = 0
			or p.uid = #column_priv1.gid
			or p.uid = #column_priv1.uid)
		and not exists (
			select * from #protects
			where
				protecttype = 206
				and action = 193
				and (name = #column_priv1.COLUMN_NAME
					or name = '~All')
				and ( uid = 0
					or uid = #column_priv1.gid
					or uid = #column_priv1.uid))

    update #column_priv1
	set insert_privilege = 1
		from #protects p
	where
		p.protecttype = 205
		and p.action = 195
		and (p.name = #column_priv1.COLUMN_NAME
			or name = '~All')
		and (p.uid = 0
			or p.uid = #column_priv1.gid
			or p.uid = #column_priv1.uid)
		and not exists (
			select * from #protects
			where
				protecttype = 206
				and action = 195
				and (name = #column_priv1.COLUMN_NAME
       				or name = '~All')
				and (uid = 0
					or uid = #column_priv1.gid
					or uid = #column_priv1.uid))

    update #column_priv1
	set update_privilege = 1
		from #protects p
	where
		p.protecttype = 205
		and p.action = 197
		and (p.name = #column_priv1.COLUMN_NAME
			or name = '~All')
		and (p.uid = 0
			or p.uid = #column_priv1.gid
			or p.uid = #column_priv1.uid)
		and not exists (
			select * from #protects
				where protecttype = 206
				and action = 197
				and (name = #column_priv1.COLUMN_NAME
					or name = '~All')
				and (uid = 0
					or uid = #column_priv1.gid
					or uid = #column_priv1.uid))

    update #column_priv1
	set references_privilege = 1
		from #protects p
	where
		p.protecttype = 205
		and p.action = 26
		and (p.name = #column_priv1.COLUMN_NAME
			or name = '~All')
		and (p.uid = 0
			or p.uid = #column_priv1.gid
			or p.uid = #column_priv1.uid)
		and not exists (
			select * from #protects
				where protecttype = 206
				and action = 26
				and (name = #column_priv1.COLUMN_NAME
					or name = '~All')
				and (uid = 0
					or uid = #column_priv1.gid
					or uid = #column_priv1.uid))

    update #column_priv1
	set select_grantable = 1
		from #protects p
	where
		p.protecttype = 204
		and p.action = 193
		and (p.name = #column_priv1.COLUMN_NAME
			or name = '~All')
		and (p.uid = 0
			or p.uid = #column_priv1.gid
			or p.uid = #column_priv1.uid)
		and not exists (
			select * from #protects
			where
				protecttype = 206
				and action = 193
				and (name = #column_priv1.COLUMN_NAME
					or name = '~All')
				and ( uid = 0
					or uid = #column_priv1.gid
					or uid = #column_priv1.uid))

    update #column_priv1
	set insert_grantable = 1
		from #protects p
	where
		p.protecttype = 204
		and p.action = 195
		and (p.name = #column_priv1.COLUMN_NAME
			or name = '~All')
		and (p.uid = 0
			or p.uid = #column_priv1.gid
			or p.uid = #column_priv1.uid)
		and not exists (
			select * from #protects
			where
				protecttype = 206
				and action = 195
				and (name = #column_priv1.COLUMN_NAME
					or name = '~All')
				and ( uid = 0
					or uid = #column_priv1.gid
					or uid = #column_priv1.uid))

    update #column_priv1
	set update_grantable = 1
		from #protects p
	where
		p.protecttype = 204
		and p.action = 197
		and (p.name = #column_priv1.COLUMN_NAME
			or name = '~All')
		and (p.uid = 0
			or p.uid = #column_priv1.gid
			or p.uid = #column_priv1.uid)
		and not exists (
			select * from #protects
			where
				protecttype = 206
				and action = 197
				and (name = #column_priv1.COLUMN_NAME
					or name = '~All')
				and ( uid = 0
					or uid = #column_priv1.gid
					or uid = #column_priv1.uid))

    update #column_priv1
	set references_grantable = 1
		from #protects p
	where
		p.protecttype = 204
		and p.action = 26
		and (p.name = #column_priv1.COLUMN_NAME
			or name = '~All')
		and (p.uid = 0
			or p.uid = #column_priv1.gid
			or p.uid = #column_priv1.uid)
		and not exists (
			select * from #protects
			where
				protecttype = 206
				and action = 26
				and (name = #column_priv1.COLUMN_NAME
					or name = '~All')
				and ( uid = 0
					or uid = #column_priv1.gid
					or uid = #column_priv1.uid))

	create table #column_priv2(
		COLUMN_NAME 	varchar(32) NOT NULL,
		grantor 		smallint NULL,
		grantee 		smallint NOT NULL,
		PRIVILEGE		varchar(32) NOT NULL,
		IS_GRANTABLE	varchar(3) NULL)

	insert into #column_priv2
		select
			COLUMN_NAME,
			grantor,
			grantee,
			'SELECT',
			'NO'
		from #column_priv1
		where select_privilege = 1 and select_grantable	= 0

	insert into #column_priv2
		select
			COLUMN_NAME,
			grantor,
			grantee,
			'INSERT',
			'NO'
		from #column_priv1
		where insert_privilege = 1 and insert_grantable = 0

	insert into #column_priv2
		select
			COLUMN_NAME,
			grantor,
			grantee,
			'UPDATE',
			'NO'
		from #column_priv1
		where update_privilege = 1 and update_grantable = 0

	insert into #column_priv2
		select
			COLUMN_NAME,
			grantor,
			grantee,
			'REFERENCES',
			'NO'
		from #column_priv1
		where references_privilege = 1 and references_grantable = 0

	insert into #column_priv2
		select
			COLUMN_NAME,
			grantor,
			grantee,
			'SELECT',
			'YES'
		from #column_priv1
		where select_grantable = 1

	insert into #column_priv2
		select
			COLUMN_NAME,
			grantor,
			grantee,
			'INSERT',
			'YES'
		from #column_priv1
		where insert_grantable = 1

	insert into #column_priv2
		select
			COLUMN_NAME,
			grantor,
			grantee,
			'UPDATE',
			'YES'
		from #column_priv1
		where update_grantable = 1

	insert into #column_priv2
		select
			COLUMN_NAME,
			grantor,
			grantee,
			'REFERENCES',
			'YES'
		from #column_priv1
		where references_grantable = 1

	select
		convert(varchar(32),db_name()) TABLE_QUALIFIER,
		convert(varchar(32),user_name(@owner_uid)) TABLE_OWNER,
		@table_name TABLE_NAME,
		COLUMN_NAME,
		convert(varchar(32),user_name(grantor)) GRANTOR,
		convert(varchar(32),user_name(grantee)) GRANTEE,
		PRIVILEGE,
		IS_GRANTABLE
	from #column_priv2
	where COLUMN_NAME like @column_name
	order by 4, 7
go

if (charindex('6.50', @@version) = 0 and
	charindex('7.00', @@version) = 0)
begin
	print ''
	print ''
	print 'Warning:'
	print 'you are installing the stored procedures '
	print 'on a pre 6.50 SQL Server.'
	print 'Ignore the following errors.'
end
else
	drop proc sp_column_privileges
go

/*	Procedure for 6.50 server */
CREATE PROCEDURE sp_column_privileges (
			@table_name 		varchar(32),
			@table_owner		varchar(32) = null,
			@table_qualifier	varchar(32) = null,
			@column_name		varchar(90) = null)
as

	declare @table_id	 int

	if @column_name is null /*	If column name not supplied, match all */
		select @column_name = '%'

	if @table_qualifier is not null
    begin
		if db_name() <> @table_qualifier
		begin	/* If qualifier doesn't match current database */
			raiserror 20001 'Table qualifier must be name of current database'
			return
		end
    end
	if @table_owner is null
	begin	/* If unqualified table name */
		select @table_id = object_id(@table_name)
    end
    else
	begin	/* Qualified table name */
		select @table_id = object_id(@table_owner + '.' + @table_name)
    end

	select
		convert(varchar(32),db_name()) TABLE_QUALIFIER,
		convert(varchar(32),user_name(o.uid)) TABLE_OWNER,
		@table_name TABLE_NAME,
		convert(varchar(32),c.name) COLUMN_NAME,
		convert(varchar(32),user_name(p.grantor)) GRANTOR,
		convert(varchar(32),user_name(u.uid)) GRANTEE,
		convert (varchar(32),case p.action
			 when 193 then 'SELECT'
			 when 195 then 'INSERT'
			 when 197 then 'UPDATE'
			 else 'REFERENCES'
		end) PRIVILEGE,
		convert (varchar(3),case when p.protecttype = 205 then 'NO'
			else 'YES'
		end) IS_GRANTABLE
	from sysprotects p, sysobjects o, sysusers u, master.dbo.spt_values v, syscolumns c
	where
		c.id = @table_id
		and c.name like @column_name
		and c.id = p.id
		and c.id = o.id
		and case substring(p.columns, 1, 1) & 1
				when NULL then 255	/* all columns have permission */
				when 0 then convert(tinyint, substring(p.columns, v.low, 1))
				else (~convert(tinyint, isnull(substring(p.columns, v.low, 1),0)))
			end
			& v.high <> 0			/* permission applies to this column */
		and v.number <= (select count(*) from syscolumns
			where id = @table_id)	/* ranges from 1 to # of columns in table */
		and v.type = 'P'
		and v.number = c.colid
			/* expand groups */
		and ((p.uid = u.uid and u.uid <> u.gid) or
			 (p.uid = u.gid and u.uid <> u.gid))
		and p.protecttype <> 206	/* only grant rows */
		and p.action in (26,193,195,197)
		and o.uid <> u.uid			/* no rows for owner */
		and not exists (			/* exclude revoke'd privileges */
			select *
			from sysprotects p1
			where
				p1.protecttype = 206
				and p1.action = p.action
				and p1.id = p.id
				and p1.uid = u.uid
				and case substring(p1.columns, 1, 1) & 1
						when NULL then 255	/* all columns have permission */
						when 0 then convert(tinyint, substring(p1.columns, v.low, 1))
                                        	else (~convert(tinyint,isnull(substring(p.columns, v.low, 1),0)))
					end
					& v.high <> 0)			/* permission applies to this column */
	union all
	select	/*	Add rows for table owner */
		convert(varchar(32),db_name()) TABLE_QUALIFIER,
		convert(varchar(32),user_name(o.uid)) TABLE_OWNER,
		@table_name TABLE_NAME,
		convert(varchar(32),col_name(@table_id, c.colid)) COLUMN_NAME,
		convert(varchar(32),user_name(u.uid)) grantor,
		convert(varchar(32),user_name(o.uid)) grantee,
		convert (varchar(32),case v.number
			when 193 then 'SELECT'
			when 195 then 'INSERT'
			when 197 then 'UPDATE'
			else 'REFERENCES'
		end) PRIVILEGE,
		convert(varchar(3),'YES') IS_GRANTABLE
	from sysobjects o, spt_values v, sysusers u, syscolumns c
	where
		c.id = @table_id
		and c.name like @column_name
		and c.id = o.id
		and u.suid = 1		/* grantor is dbo of database */
		and v.type = 'P'	/* cross product to get all exposed privileges */
		and v.number in (26,193,195,197)
		and not exists (	/* exclude revoke'd privileges */
			select *
			from sysprotects p1
			where
				p1.protecttype = 206
				and p1.action = v.number
				and p1.id = o.id
				and p1.uid = o.uid)
	order by 4, 7
go

grant execute on sp_column_privileges to public
go

dump tran master with no_log
go

print 'creating sp_columns'
go

/*	Procedure for pre-6.0 server */
CREATE PROCEDURE sp_columns (
				 @table_name		varchar(90),
				 @table_owner		varchar(90) = null,
				 @table_qualifier	varchar(90) = null,
				 @column_name		varchar(90) = null,
				 @ODBCVer			int = 2)
AS
	DECLARE @full_table_name	char(181)
    DECLARE @table_id int

	if @ODBCVer <> 3
		select @ODBCVer = 2
	if @column_name is null /*	If column name not supplied, match all */
		select @column_name = '%'
	if @table_qualifier is not null
    begin
		if db_name() <> @table_qualifier
		begin	/* If qualifier doesn't match current database */
			raiserror 20001 'Table qualifier must be name of current database'
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
		@table_id <> 0)
    begin
		/* this block is for the case where there is no pattern
			 matching required for the table name */
		SELECT
			TABLE_QUALIFIER = convert(varchar(32),DB_NAME()),
			TABLE_OWNER = convert(varchar(32),USER_NAME(o.uid)),
			TABLE_NAME = convert(varchar(32),o.name),
			COLUMN_NAME = convert(varchar(32),c.name),
			d.DATA_TYPE,
			TYPE_NAME = t.name,
			"PRECISION" = isnull(d.data_precision, convert(int,c.length)),
			LENGTH = isnull(d.length, convert(int,c.length)),
			SCALE = d.numeric_scale,
			d.RADIX,
			NULLABLE =	/* set nullability from status flag */
				convert(smallint, convert(bit, c.status&8)),
			REMARKS = convert(varchar(254),null),	/* Remarks are NULL */
			COLUMN_DEF = convert(varchar(254),substring(text,2,datalength(text)-2)),
			d.SQL_DATA_TYPE,
			d.SQL_DATETIME_SUB,
			CHAR_OCTET_LENGTH = isnull(d.data_precision, convert(int,c.length))+d.charbin,
			ORDINAL_POSITION = convert(int,c.colid),
			IS_NULLABLE = convert(varchar(254),rtrim(substring('NO      YES',(c.status&8)+1,3))),
			SS_DATA_TYPE = c.type
		FROM
			syscolumns c,
			sysobjects o,
			syscomments m,
			master.dbo.spt_datatype_info d,
			systypes t
		WHERE
			o.id = @table_id
			AND c.id = o.id
			AND t.type = d.ss_dtype
			AND c.length = isnull(d.fixlen, c.length)
			AND (d.ODBCVer is null or d.ODBCVer = @ODBCVer)
			AND o.type <> 'P'
			AND c.usertype = t.usertype
			AND c.name like @column_name
			AND c.cdefault *= m.id
		ORDER BY 17
	end
	else
    begin
		/* this block is for the case where there IS pattern
			 matching done on the table name */
		if @table_owner is null /*	If owner not supplied, match all */
			select @table_owner = '%'
		SELECT
			TABLE_QUALIFIER = convert(varchar(32),DB_NAME()),
			TABLE_OWNER = convert(varchar(32),USER_NAME(o.uid)),
			TABLE_NAME = convert(varchar(32),o.name),
			COLUMN_NAME = convert(varchar(32),c.name),
			d.DATA_TYPE,
			TYPE_NAME = t.name,
			"PRECISION" = isnull(d.data_precision, convert(int,c.length)),
			LENGTH = isnull(d.length, convert(int,c.length)),
			SCALE = d.numeric_scale,
			d.RADIX,
			NULLABLE =	/* set nullability from status flag */
				convert(smallint, convert(bit, c.status&8)),
			REMARKS = convert(varchar(254),null),	/* Remarks are NULL */
			COLUMN_DEF = convert(varchar(254),substring(text,2,datalength(text)-2)),
			d.SQL_DATA_TYPE,
			d.SQL_DATETIME_SUB,
			CHAR_OCTET_LENGTH = isnull(d.data_precision, convert(int,c.length))+d.charbin,
			ORDINAL_POSITION = convert(int,c.colid),
			IS_NULLABLE = convert(varchar(254),rtrim(substring('NO      YES',(c.status&8)+1,3))),
			SS_DATA_TYPE = c.type
		FROM
			syscolumns c,
			sysobjects o,
			syscomments m,
			master.dbo.spt_datatype_info d,
			systypes t
		WHERE
			o.name like @table_name
			AND user_name(o.uid) like @table_owner
			AND o.id = c.id
			AND t.type = d.ss_dtype
			AND c.length = isnull(d.fixlen, c.length)
			AND (d.ODBCVer is null or d.ODBCVer = @ODBCVer)
			AND o.type <> 'P'
			AND c.usertype = t.usertype
			AND c.name like @column_name
			AND c.cdefault *= m.id
		ORDER BY 2, 3, 17
	end
go

if (charindex('6.00', @@version) = 0 and
	charindex('6.50', @@version) = 0 and
	charindex('7.00', @@version) = 0)
begin
	print ''
	print ''
	print 'Warning:'
	print 'you are installing the stored procedures '
	print 'on a pre 6.0 SQL Server.'
	print 'Ignore the following error.'
end
else
	drop proc sp_columns
go

/*	Procedure for 6.0 server */
CREATE PROCEDURE sp_columns (
				 @table_name		varchar(90),
				 @table_owner		varchar(90) = null,
				 @table_qualifier	varchar(90) = null,
				 @column_name		varchar(90) = null,
				 @ODBCVer			int = 2)
AS
	DECLARE @full_table_name	char(181)
    DECLARE @table_id int

	if @ODBCVer <> 3
		select @ODBCVer = 2
	if @column_name is null /*	If column name not supplied, match all */
		select @column_name = '%'
	if @table_qualifier is not null
    begin
		if db_name() <> @table_qualifier
		begin	/* If qualifier doesn't match current database */
			raiserror (15250, -1,-1,'Table')
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
		@table_id <> 0)
    begin
		/* this block is for the case where there is no pattern
			matching required for the table name */
		SELECT
			TABLE_QUALIFIER = convert(varchar(32),DB_NAME()),
			TABLE_OWNER = convert(varchar(32),USER_NAME(o.uid)),
			TABLE_NAME = convert(varchar(32),o.name),
			COLUMN_NAME = convert(varchar(32),c.name),
			d.DATA_TYPE,
			convert (varchar(30),case
				when t.usertype > 100 or t.usertype in (18,80) then t.name
				else d.TYPE_NAME
			end) TYPE_NAME,
			convert(int,case
				when d.DATA_TYPE in (6,7) then d.data_precision 		/* FLOAT/REAL */
				else isnull(convert(int,c.prec), 2147483647)
			end) "PRECISION",
			convert(int,case
				when d.ss_dtype IN (106, 108, 55, 63) then	/* decimal/numeric types */
					c.prec+2
				else
					isnull(d.length, c.length)
			end) LENGTH,
			SCALE = convert(smallint, c.scale),
			d.RADIX,
			NULLABLE =	/* set nullability from status flag */
				convert(smallint, convert(bit, c.status&8)),
			REMARKS = convert(varchar(254),null),	/* Remarks are NULL */
			COLUMN_DEF = convert(varchar(254),substring(text,2,datalength(text)-2)),
			d.SQL_DATA_TYPE,
			d.SQL_DATETIME_SUB,
			CHAR_OCTET_LENGTH = isnull(convert(int,c.prec), 2147483647)+d.charbin,
			ORDINAL_POSITION = convert(int,c.colid),
			IS_NULLABLE = convert(varchar(254),rtrim(substring('NO      YES',(c.status&8)+1,3))),
			SS_DATA_TYPE = c.type
		FROM
			syscolumns c,
			sysobjects o,
			syscomments m,
			master.dbo.spt_datatype_info d,
			systypes t
		WHERE
			o.id = @table_id
			AND c.id = o.id
			AND t.type = d.ss_dtype
			AND c.length = isnull(d.fixlen, c.length)
			AND (d.ODBCVer is null or d.ODBCVer = @ODBCVer)
			AND o.type <> 'P'
			AND isnull(d.AUTO_INCREMENT,0) = (c.status&128)/128
			AND c.usertype = t.usertype
			AND c.name like @column_name
			AND c.cdefault *= m.id
		ORDER BY 17
	end
	else
    begin
		/* this block is for the case where there IS pattern
			matching done on the table name */
		if @table_owner is null /*	If owner not supplied, match all */
			select @table_owner = '%'
		SELECT
			TABLE_QUALIFIER = convert(varchar(32),DB_NAME()),
			TABLE_OWNER = convert(varchar(32),USER_NAME(o.uid)),
			TABLE_NAME = convert(varchar(32),o.name),
			COLUMN_NAME = convert(varchar(32),c.name),
			d.DATA_TYPE,
			convert(varchar(30),case
				when t.usertype > 100 or t.usertype in (18,80) then t.name
				else d.TYPE_NAME
			end) TYPE_NAME,
			convert(int,case
				when d.DATA_TYPE in (6,7) then d.data_precision 		/* FLOAT/REAL */
				else isnull(convert(int,c.prec), 2147483647)
			end) "PRECISION",
			convert(int,case
				when d.ss_dtype IN (106, 108, 55, 63) then	/* decimal/numeric types */
					c.prec+2
				else
					isnull(d.length, c.length)
			end) LENGTH,
			SCALE = convert(smallint, c.scale),
			d.RADIX,
			NULLABLE =	/* set nullability from status flag */
				convert(smallint, convert(bit, c.status&8)),
			REMARKS = convert(varchar(254),null),	/* Remarks are NULL */
			COLUMN_DEF = convert(varchar(254),substring(text,2,datalength(text)-2)),
			d.SQL_DATA_TYPE,
			d.SQL_DATETIME_SUB,
			CHAR_OCTET_LENGTH = isnull(convert(int,c.prec), 2147483647)+d.charbin,
			ORDINAL_POSITION = convert(int,c.colid),
			IS_NULLABLE = convert(varchar(254),rtrim(substring('NO      YES',(c.status&8)+1,3))),
			SS_DATA_TYPE = c.type
		FROM
			syscolumns c,
			sysobjects o,
			syscomments m,
			master.dbo.spt_datatype_info d,
			systypes t
		WHERE
			o.name like @table_name
			AND user_name(o.uid) like @table_owner
			AND o.id = c.id
			AND t.type = d.ss_dtype
			AND c.length = isnull(d.fixlen, c.length)
			AND (d.ODBCVer is null or d.ODBCVer = @ODBCVer)
			AND o.type <> 'P'
			AND isnull(d.AUTO_INCREMENT,0) = (c.status&128)/128
			AND c.usertype = t.usertype
			AND c.name like @column_name
			AND c.cdefault *= m.id
		ORDER BY 2, 3, 17
	end
go

grant execute on sp_columns to public
go

dump tran master with no_log
go

print 'creating sp_databases'
go

create proc sp_databases
as
	/* Use temporary table to sum up database size w/o using group by */
	create table #databases (
				  DATABASE_NAME varchar(32) NOT NULL,
				  size int NOT NULL)

	/* Insert row for each database */
	insert into #databases
		select
			name,
			(select sum(size) from master.dbo.sysusages
				where dbid = d.dbid)
		from master.dbo.sysdatabases d

	select
		 convert(varchar(32),DATABASE_NAME),
		 DATABASE_SIZE = size*2,	/* Convert from 2048 byte pages to K */
		 REMARKS = convert(varchar(254),null)	/* Remarks are NULL */
	from #databases
	order by 1
go

grant execute on sp_databases to public
go

dump tran master with no_log
go

print 'creating sp_datatype_info'
go

/*	Procedure for pre-6.0 server */
create proc sp_datatype_info
	(@data_type int = 0, @ODBCVer tinyint = 2)
as
	if @ODBCVer <> 3
		select @ODBCVer = 2
	if @data_type = 0
		select
			TYPE_NAME = t.name,
			d.DATA_TYPE,
			"PRECISION" = isnull(d.data_precision, convert(int,t.length)),
			d.LITERAL_PREFIX,
			d.LITERAL_SUFFIX,
			e.CREATE_PARAMS,
			d.NULLABLE,
			d.CASE_SENSITIVE,
			d.SEARCHABLE,
			d.UNSIGNED_ATTRIBUTE,
			d.MONEY,
			d.AUTO_INCREMENT,
			LOCAL_TYPE_NAME = t.name,
			MINIMUM_SCALE = d.numeric_scale,
			MAXIMUM_SCALE = d.numeric_scale,
			d.SQL_DATA_TYPE,
			d.SQL_DATETIME_SUB,
			NUM_PREC_RADIX = convert(int,d.RADIX),
			INTERVAL_PRECISION = convert(smallint,NULL),
			USERTYPE = t.usertype
		from master.dbo.spt_datatype_info d, master.dbo.spt_datatype_info_ext e, systypes t
		where
			d.ss_dtype = t.type
			AND (d.ODBCVer is null or d.ODBCVer = @ODBCVer)
			and t.usertype *= e.user_type
			and t.type not in (111,109,38,110)	/* get rid of nullable types */
		order by 2, 12, 11, t.usertype

	else
		select
			TYPE_NAME = t.name,
			d.DATA_TYPE,
			"PRECISION" = isnull(d.data_precision, convert(int,t.length)),
			d.LITERAL_PREFIX,
			d.LITERAL_SUFFIX,
			e.CREATE_PARAMS,
			d.NULLABLE,
			d.CASE_SENSITIVE,
			d.SEARCHABLE,
			d.UNSIGNED_ATTRIBUTE,
			d.MONEY,
			d.AUTO_INCREMENT,
			LOCAL_TYPE_NAME = t.name,
			MINIMUM_SCALE = d.numeric_scale,
			MAXIMUM_SCALE = d.numeric_scale,
			d.SQL_DATA_TYPE,
			d.SQL_DATETIME_SUB,
			NUM_PREC_RADIX = convert(int,d.RADIX),
			INTERVAL_PRECISION = convert(smallint,NULL),
			USERTYPE = t.usertype
		from master.dbo.spt_datatype_info d, master.dbo.spt_datatype_info_ext e, systypes t
		where
			DATA_TYPE = @data_type
			and d.ss_dtype = t.type
			AND (d.ODBCVer is null or d.ODBCVer = @ODBCVer)
			and t.usertype *= e.user_type
			and t.type not in (111,109,38,110)	/* get rid of nullable types */
		order by 12, 11, t.usertype

go

if (charindex('6.00', @@version) = 0 and
	charindex('6.50', @@version) = 0 and
	charindex('7.00', @@version) = 0)
begin
	print ''
	print ''
	print 'Warning:'
	print 'you are installing the stored procedures '
	print 'on a pre 6.0 SQL Server.'
	print 'Ignore the following errors.'
end
else
	drop proc sp_datatype_info
go

/*	Procedure for 6.0 server */
create proc sp_datatype_info
	(@data_type int = 0, @ODBCVer tinyint = 2)
as
	if @ODBCVer <> 3
		select @ODBCVer = 2
	if @data_type = 0
		select
			convert(varchar(30),case
				when t.usertype > 100 or t.usertype in (18,80) then t.name
				else d.TYPE_NAME
			end) TYPE_NAME,
			d.DATA_TYPE,
			convert(int,case
				when d.DATA_TYPE in (6,7) then d.data_precision 		/* FLOAT/REAL */
				when d.ss_dtype in (35,34) then 2147483647				/* TEXT/IMAGE */
				when d.ss_dtype in (55,63,106,108) then @@max_precision /* DECIMAL/NUMERIC */
				else t.prec
			end) "PRECISION",
			d.LITERAL_PREFIX,
			d.LITERAL_SUFFIX,
			e.CREATE_PARAMS,
			convert(smallint,case
				when d.AUTO_INCREMENT = 1 then 0 /* IDENTITY*/
				else t.allownulls
			end) NULLABLE,
			d.CASE_SENSITIVE,
			d.SEARCHABLE,
			d.UNSIGNED_ATTRIBUTE,
			d.MONEY,
			d.AUTO_INCREMENT,
			convert(varchar(30),case
				when t.usertype > 100 or t.usertype in (18,80) then t.name
				else d.TYPE_NAME
			end) LOCAL_TYPE_NAME,
			MINIMUM_SCALE = d.numeric_scale,
			convert(smallint,case
				when d.ss_dtype in (106,108) and d.AUTO_INCREMENT = 0 then @@max_precision /* DECIMAL/NUMERIC */
				when d.ss_dtype in (106,108) and d.AUTO_INCREMENT = 1 then 0 /* DECIMAL/NUMERIC IDENTITY*/
				else t.scale
			end) MAXIMUM_SCALE,
			d.SQL_DATA_TYPE,
			d.SQL_DATETIME_SUB,
			NUM_PREC_RADIX = convert(int,d.RADIX),
			INTERVAL_PRECISION = convert(smallint,NULL),
			USERTYPE = t.usertype
		from master.dbo.spt_datatype_info d, master.dbo.spt_datatype_info_ext e, systypes t
		where
			d.ss_dtype = t.type
			AND (d.ODBCVer is null or d.ODBCVer = @ODBCVer)
			and t.usertype *= e.user_type
			and isnull(d.AUTO_INCREMENT,0) *= e.AUTO_INCREMENT
			and t.type not in (111,109,38,110,55,63)	/* get rid of nullable types */
		order by 2, 12, 11, t.usertype

	else
		select
			convert (varchar(30),case
				when t.usertype > 100 or t.usertype in (18,80) then t.name
				else d.TYPE_NAME
			end) TYPE_NAME,
			d.DATA_TYPE,
			convert(int,case
				when d.DATA_TYPE in (6,7) then d.data_precision 		/* FLOAT/REAL */
				when d.ss_dtype in (35,34) then 2147483647				/* TEXT/IMAGE */
				when d.ss_dtype in (55,63,106,108) then @@max_precision /* DECIMAL/NUMERIC */
				else t.prec
			end) "PRECISION",
			d.LITERAL_PREFIX,
			d.LITERAL_SUFFIX,
			e.CREATE_PARAMS,
			convert(smallint,case
				when d.AUTO_INCREMENT = 1 then 0 /* IDENTITY*/
				else t.allownulls
			end) NULLABLE,
			d.CASE_SENSITIVE,
			d.SEARCHABLE,
			d.UNSIGNED_ATTRIBUTE,
			d.MONEY,
			d.AUTO_INCREMENT,
			convert(varchar(30),case
				when t.usertype > 100 or t.usertype in (18,80) then t.name
				else d.TYPE_NAME
			end) LOCAL_TYPE_NAME,
			MINIMUM_SCALE = d.numeric_scale,
			convert(smallint,case
				when d.ss_dtype in (106,108) and d.AUTO_INCREMENT = 0 then convert(smallint,@@max_precision) /* DECIMAL/NUMERIC */
				when d.ss_dtype in (106,108) and d.AUTO_INCREMENT = 1 then convert(smallint,0) /* DECIMAL/NUMERIC IDENTITY*/
				else t.scale
			end) MAXIMUM_SCALE,
			d.SQL_DATA_TYPE,
			d.SQL_DATETIME_SUB,
			NUM_PREC_RADIX = convert(int,d.RADIX),
			INTERVAL_PRECISION = convert(smallint,NULL),
			USERTYPE = t.usertype
		from master.dbo.spt_datatype_info d, master.dbo.spt_datatype_info_ext e, systypes t
		where
			d.DATA_TYPE = @data_type
			and d.ss_dtype = t.type
			AND (d.ODBCVer is null or d.ODBCVer = @ODBCVer)
			and t.usertype *= e.user_type
			and isnull(d.AUTO_INCREMENT,0) *= e.AUTO_INCREMENT
			and t.type not in (111,109,38,110,55,63)	/* get rid of nullable types */
		order by 12, 11, t.usertype
go

grant execute on sp_datatype_info to public
go

dump tran master with no_log
go

print 'creating sp_fkeys'
go

/*	Procedure for pre-6.0 server */
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
		raiserror 20004 'PK table name or FK table name must be given.'
		return
    end
	if @fktable_qualifier is not null
    begin
		if db_name() <> @fktable_qualifier
		begin	/* If qualifier doesn't match current database */
			raiserror 20001 'Foreign Key Table qualifier must be name of current database'
			return
		end
    end
	if @pktable_qualifier is not null
    begin
		if db_name() <> @pktable_qualifier
		begin	/* If qualifier doesn't match current database */
			raiserror 20001 'Primary Key Table qualifier must be name of current database'
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

	if @@trancount <> 0
	begin	/* If inside a transaction */
		raiserror 20003 'The procedure ''sp_fkeys'' cannot be executed from within a transaction.'
		return
    end
	create table #fkeys(
			 PKTABLE_QUALIFIER	varchar(32) NULL,
			 PKTABLE_OWNER		varchar(32) NULL,
			 PKTABLE_NAME		varchar(32) NOT NULL,
			 PKCOLUMN_NAME		varchar(32) NOT NULL,
			 FKTABLE_QUALIFIER	varchar(32) NULL,
			 FKTABLE_OWNER		varchar(32) NULL,
			 FKTABLE_NAME		varchar(32) NOT NULL,
			 FKCOLUMN_NAME		varchar(32) NOT NULL,
			 KEY_SEQ			smallint NOT NULL)

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
	union all
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
	union all
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
	union all
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
	union all
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
	union all
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
	union all
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
	union all
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
			PKTABLE_QUALIFIER,
			PKTABLE_OWNER,
			PKTABLE_NAME,
			PKCOLUMN_NAME,
			FKTABLE_QUALIFIER,
			FKTABLE_OWNER,
			FKTABLE_NAME,
			FKCOLUMN_NAME,
			KEY_SEQ,
			UPDATE_RULE = convert(smallint, null),
			DELETE_RULE = convert(smallint,null),
			FK_NAME = convert(varchar(32),null),
			PK_NAME = convert(varchar(32),null)
		from #fkeys
		where FKTABLE_NAME like @fktable_name
			and FKTABLE_OWNER like @fktable_owner
			and PKTABLE_NAME  like @pktable_name
			and PKTABLE_OWNER like @pktable_owner
		order by 1, 2, 3, 9
	else		/*	Order by FK fields */
		select
			PKTABLE_QUALIFIER,
			PKTABLE_OWNER,
			PKTABLE_NAME,
			PKCOLUMN_NAME,
			FKTABLE_QUALIFIER,
			FKTABLE_OWNER,
			FKTABLE_NAME,
			FKCOLUMN_NAME,
			KEY_SEQ,
			UPDATE_RULE = convert(smallint,null),
			DELETE_RULE = convert(smallint,null),
			FK_NAME = convert(varchar(32),null),
			PK_NAME = convert(varchar(32),null)
		from #fkeys
		where FKTABLE_NAME like @fktable_name
			and FKTABLE_OWNER like @fktable_owner
			and PKTABLE_NAME  like @pktable_name
			and PKTABLE_OWNER like @pktable_owner
		order by 5, 6, 7, 9
go

if (charindex('6.00', @@version) = 0 and
	charindex('6.50', @@version) = 0 and
	charindex('7.00', @@version) = 0)
begin
	print ''
	print ''
	print 'Warning:'
	print 'you are installing the stored procedures '
	print 'on a pre 6.0 SQL Server.'
	print 'Ignore the following errors.'
end
else
	drop proc sp_fkeys
go

/*	Procedure for 6.0 server */
CREATE PROCEDURE sp_fkeys(
			   @pktable_name		varchar(32) = null,
			   @pktable_owner		varchar(32) = null,
			   @pktable_qualifier	varchar(32) = null,
			   @fktable_name		varchar(32) = null,
			   @fktable_owner		varchar(32) = null,
			   @fktable_qualifier	varchar(32) = null )
as
	DECLARE @pktable_id			int
	DECLARE @pkfull_table_name	char(70)
	DECLARE @fktable_id			int
	DECLARE @fkfull_table_name	char(70)
	declare	@order_by_pk		int

    select  @order_by_pk = 0

	if (@pktable_name is null) and (@fktable_name is null)
	begin	/* If neither primary key nor foreign key table names given */
		raiserror (15252,-1,-1)
		return
    end

	if @pktable_owner is null
	begin	/* If unqualified primary key table name */
		SELECT @pkfull_table_name = @pktable_name
    end
    else
	begin	/* Qualified primary key table name */
		SELECT @pkfull_table_name = @pktable_owner + '.' + @pktable_name
    end
	/*	Get Object ID */
	SELECT @pktable_id = object_id(@pkfull_table_name)

	if @fktable_owner is null
	begin	/* If unqualified foreign key table name */
		SELECT @fkfull_table_name = @fktable_name
    end
    else
	begin	/* Qualified foreign key table name */
		SELECT @fkfull_table_name = @fktable_owner + '.' + @fktable_name
    end
	/*	Get Object ID */
	SELECT @fktable_id = object_id(@fkfull_table_name)

	if @fktable_name is not null
	begin
		if @fktable_id is null
			SELECT @fktable_id = 0	/* fk table not found, empty result */
    end

	if @pktable_name is null
	begin /*  If table name not supplied, match all */
		select @order_by_pk = 1
	end
	else
	begin
		if @pktable_id is null
		begin
			SELECT @pktable_id = 0	/* pk table not found, empty result */
		end
	end

	if (@@trancount <> 0 and
		charindex('6.50', @@version) = 0 and
		charindex('7.00', @@version) = 0)
	begin	/* If inside a transaction */
		raiserror(15002,-1,-1,'sp_fkeys')
		return
	end

	create table #fkeys(
			 pkdb_id		int NOT NULL,
			 pktable_id 	int NOT NULL,
			 pkcolid		int NOT NULL,
			 fkdb_id		int NOT NULL,
			 fktable_id		int NOT NULL,
			 fkcolid		int NOT NULL,
			 KEY_SEQ		smallint NOT NULL,
			 fk_id			int NOT NULL,
			 pk_id			int NOT NULL)

	/*	SQL Server supports upto 16 PK/FK relationships between 2 tables */
	/*	Process syskeys for each relationship */
	/*	The inserts below adds a row to the temp table for each of the
		16 possible relationships */
    insert into #fkeys
		select
			r.rkeydbid,
			r.rkeyid,
			r.rkey1,
			r.fkeydbid,
			r.fkeyid,
			r.fkey1,
			1,
			r.constid,
			s.constid
		from
			sysreferences r, sysconstraints s
		where	r.rkeyid = s.id
			AND (s.status & 0xf) = 1
			AND r.rkeyid between isnull(@pktable_id, 0) and isnull(@pktable_id, 0x7fffffff)
			AND r.fkeyid between isnull(@fktable_id, 0) and isnull(@fktable_id, 0x7fffffff)
	  union all
		select
			r.rkeydbid,
			r.rkeyid,
			r.rkey2,
			r.fkeydbid,
			r.fkeyid,
			r.fkey2,
			2,
			r.constid,
			s.constid
		from
			sysreferences r, sysconstraints s
		where	r.rkeyid = s.id
			AND (s.status & 0xf) = 1
			AND r.rkeyid between isnull(@pktable_id, 0) and isnull(@pktable_id, 0x7fffffff)
			AND r.fkeyid between isnull(@fktable_id, 0) and isnull(@fktable_id, 0x7fffffff)
	  union all
		select
			r.rkeydbid,
			r.rkeyid,
			r.rkey3,
			r.fkeydbid,
			r.fkeyid,
			r.fkey3,
			3,
			r.constid,
			s.constid
		from
			sysreferences r, sysconstraints s
		where	r.rkeyid = s.id
			AND (s.status & 0xf) = 1
			AND r.rkeyid between isnull(@pktable_id, 0) and isnull(@pktable_id, 0x7fffffff)
			AND r.fkeyid between isnull(@fktable_id, 0) and isnull(@fktable_id, 0x7fffffff)
	  union all
		select
			r.rkeydbid,
			r.rkeyid,
			r.rkey4,
			r.fkeydbid,
			r.fkeyid,
			r.fkey4,
			4,
			r.constid,
			s.constid
		from
			sysreferences r, sysconstraints s
		where	r.rkeyid = s.id
			AND (s.status & 0xf) = 1
			AND r.rkeyid between isnull(@pktable_id, 0) and isnull(@pktable_id, 0x7fffffff)
			AND r.fkeyid between isnull(@fktable_id, 0) and isnull(@fktable_id, 0x7fffffff)
	  union all
		select
			r.rkeydbid,
			r.rkeyid,
			r.rkey5,
			r.fkeydbid,
			r.fkeyid,
			r.fkey5,
			5,
			r.constid,
			s.constid
		from
			sysreferences r, sysconstraints s
		where	r.rkeyid = s.id
			AND (s.status & 0xf) = 1
			AND r.rkeyid between isnull(@pktable_id, 0) and isnull(@pktable_id, 0x7fffffff)
			AND r.fkeyid between isnull(@fktable_id, 0) and isnull(@fktable_id, 0x7fffffff)
	  union all
		select
			r.rkeydbid,
			r.rkeyid,
			r.rkey6,
			r.fkeydbid,
			r.fkeyid,
			r.fkey6,
			6,
			r.constid,
			s.constid
		from
			sysreferences r, sysconstraints s
		where	r.rkeyid = s.id
			AND (s.status & 0xf) = 1
			AND r.rkeyid between isnull(@pktable_id, 0) and isnull(@pktable_id, 0x7fffffff)
			AND r.fkeyid between isnull(@fktable_id, 0) and isnull(@fktable_id, 0x7fffffff)
	  union all
		select
			r.rkeydbid,
			r.rkeyid,
			r.rkey7,
			r.fkeydbid,
			r.fkeyid,
			r.fkey7,
			7,
			r.constid,
			s.constid
		from
			sysreferences r, sysconstraints s
		where	r.rkeyid = s.id
			AND (s.status & 0xf) = 1
			AND r.rkeyid between isnull(@pktable_id, 0) and isnull(@pktable_id, 0x7fffffff)
			AND r.fkeyid between isnull(@fktable_id, 0) and isnull(@fktable_id, 0x7fffffff)
	  union all
		select
			r.rkeydbid,
			r.rkeyid,
			r.rkey8,
			r.fkeydbid,
			r.fkeyid,
			r.fkey8,
			8,
			r.constid,
			s.constid
		from
			sysreferences r, sysconstraints s
		where	r.rkeyid = s.id
			AND (s.status & 0xf) = 1
			AND r.rkeyid between isnull(@pktable_id, 0) and isnull(@pktable_id, 0x7fffffff)
			AND r.fkeyid between isnull(@fktable_id, 0) and isnull(@fktable_id, 0x7fffffff)
	  union all
		select
			r.rkeydbid,
			r.rkeyid,
			r.rkey9,
			r.fkeydbid,
			r.fkeyid,
			r.fkey9,
			9,
			r.constid,
			s.constid
		from
			sysreferences r, sysconstraints s
		where	r.rkeyid = s.id
			AND (s.status & 0xf) = 1
			AND r.rkeyid between isnull(@pktable_id, 0) and isnull(@pktable_id, 0x7fffffff)
			AND r.fkeyid between isnull(@fktable_id, 0) and isnull(@fktable_id, 0x7fffffff)
	  union all
		select
			r.rkeydbid,
			r.rkeyid,
			r.rkey10,
			r.fkeydbid,
			r.fkeyid,
			r.fkey10,
			10,
			r.constid,
			s.constid
		from
			sysreferences r, sysconstraints s
		where	r.rkeyid = s.id
			AND (s.status & 0xf) = 1
			AND r.rkeyid between isnull(@pktable_id, 0) and isnull(@pktable_id, 0x7fffffff)
			AND r.fkeyid between isnull(@fktable_id, 0) and isnull(@fktable_id, 0x7fffffff)
	  union all
		select
			r.rkeydbid,
			r.rkeyid,
			r.rkey11,
			r.fkeydbid,
			r.fkeyid,
			r.fkey11,
			11,
			r.constid,
			s.constid
		from
			sysreferences r, sysconstraints s
		where	r.rkeyid = s.id
			AND (s.status & 0xf) = 1
			AND r.rkeyid between isnull(@pktable_id, 0) and isnull(@pktable_id, 0x7fffffff)
			AND r.fkeyid between isnull(@fktable_id, 0) and isnull(@fktable_id, 0x7fffffff)
	  union all
		select
			r.rkeydbid,
			r.rkeyid,
			r.rkey12,
			r.fkeydbid,
			r.fkeyid,
			r.fkey12,
			12,
			r.constid,
			s.constid
		from
			sysreferences r, sysconstraints s
		where	r.rkeyid = s.id
			AND (s.status & 0xf) = 1
			AND r.rkeyid between isnull(@pktable_id, 0) and isnull(@pktable_id, 0x7fffffff)
			AND r.fkeyid between isnull(@fktable_id, 0) and isnull(@fktable_id, 0x7fffffff)
	  union all
		select
			r.rkeydbid,
			r.rkeyid,
			r.rkey13,
			r.fkeydbid,
			r.fkeyid,
			r.fkey13,
			13,
			r.constid,
			s.constid
		from
			sysreferences r, sysconstraints s
		where	r.rkeyid = s.id
			AND (s.status & 0xf) = 1
			AND r.rkeyid between isnull(@pktable_id, 0) and isnull(@pktable_id, 0x7fffffff)
			AND r.fkeyid between isnull(@fktable_id, 0) and isnull(@fktable_id, 0x7fffffff)
	  union all
		select
			r.rkeydbid,
			r.rkeyid,
			r.rkey14,
			r.fkeydbid,
			r.fkeyid,
			r.fkey14,
			14,
			r.constid,
			s.constid
		from
			sysreferences r, sysconstraints s
		where	r.rkeyid = s.id
			AND (s.status & 0xf) = 1
			AND r.rkeyid between isnull(@pktable_id, 0) and isnull(@pktable_id, 0x7fffffff)
			AND r.fkeyid between isnull(@fktable_id, 0) and isnull(@fktable_id, 0x7fffffff)
	  union all
		select
			r.rkeydbid,
			r.rkeyid,
			r.rkey15,
			r.fkeydbid,
			r.fkeyid,
			r.fkey15,
			15,
			r.constid,
			s.constid
		from
			sysreferences r, sysconstraints s
		where	r.rkeyid = s.id
			AND (s.status & 0xf) = 1
			AND r.rkeyid between isnull(@pktable_id, 0) and isnull(@pktable_id, 0x7fffffff)
			AND r.fkeyid between isnull(@fktable_id, 0) and isnull(@fktable_id, 0x7fffffff)
	  union all
		select
			r.rkeydbid,
			r.rkeyid,
			r.rkey16,
			r.fkeydbid,
			r.fkeyid,
			r.fkey16,
			16,
			r.constid,
			s.constid
		from
			sysreferences r, sysconstraints s
		where	r.rkeyid = s.id
			AND (s.status & 0xf) = 1
			AND r.rkeyid between isnull(@pktable_id, 0) and isnull(@pktable_id, 0x7fffffff)
			AND r.fkeyid between isnull(@fktable_id, 0) and isnull(@fktable_id, 0x7fffffff)

	if @order_by_pk = 1 /*	If order by PK fields */
		select
			PKTABLE_QUALIFIER = convert(varchar(32),DB_NAME(f.pkdb_id)),
			PKTABLE_OWNER = convert(varchar(32),USER_NAME(o1.uid)),
			PKTABLE_NAME = convert(varchar(32),o1.name),
			PKCOLUMN_NAME = convert(varchar(32),c1.name),
			FKTABLE_QUALIFIER = convert(varchar(32),DB_NAME(f.fkdb_id)),
			FKTABLE_OWNER = convert(varchar(32),USER_NAME(o2.uid)),
			FKTABLE_NAME = convert(varchar(32),o2.name),
			FKCOLUMN_NAME = convert(varchar(32),c2.name),
			KEY_SEQ,
			UPDATE_RULE = convert(smallint,1),
			DELETE_RULE = convert(smallint,1),
			FK_NAME = convert(varchar(128),OBJECT_NAME(fk_id)),
			PK_NAME = convert(varchar(128),OBJECT_NAME(pk_id))
		from #fkeys f,
			sysobjects o1, sysobjects o2,
			syscolumns c1, syscolumns c2
		where	o1.id = f.pktable_id
			AND o2.id = f.fktable_id
			AND c1.id = f.pktable_id
			AND c2.id = f.fktable_id
			AND c1.colid = f.pkcolid
			AND c2.colid = f.fkcolid
		order by 1,2,3,9
	else		/*	Order by FK fields */
		select
			PKTABLE_QUALIFIER = convert(varchar(32),DB_NAME(f.pkdb_id)),
			PKTABLE_OWNER = convert(varchar(32),USER_NAME(o1.uid)),
			PKTABLE_NAME = convert(varchar(32),o1.name),
			PKCOLUMN_NAME = convert(varchar(32),c1.name),
			FKTABLE_QUALIFIER = convert(varchar(32),DB_NAME(f.fkdb_id)),
			FKTABLE_OWNER = convert(varchar(32),USER_NAME(o2.uid)),
			FKTABLE_NAME = convert(varchar(32),o2.name),
			FKCOLUMN_NAME = convert(varchar(32),c2.name),
			KEY_SEQ,
			UPDATE_RULE = convert(smallint,1),
			DELETE_RULE = convert(smallint,1),
			FK_NAME = convert(varchar(128),OBJECT_NAME(fk_id)),
			PK_NAME = convert(varchar(128),OBJECT_NAME(pk_id))
		from #fkeys f,
			sysobjects o1, sysobjects o2,
			syscolumns c1, syscolumns c2
		where	o1.id = f.pktable_id
			AND o2.id = f.fktable_id
			AND c1.id = f.pktable_id
			AND c2.id = f.fktable_id
			AND c1.colid = f.pkcolid
			AND c2.colid = f.fkcolid
		order by 5,6,7,9
go

grant execute on sp_fkeys to public
go

dump tran master with no_log
go

print 'creating sp_pkeys'
go

/*	Procedure for pre-6.0 server */
CREATE PROCEDURE sp_pkeys(
			   @table_name		varchar(32),
			   @table_owner 	varchar(32) = null,
			   @table_qualifier varchar(32) = null )
as
	if @table_qualifier is not null
    begin
		if db_name() <> @table_qualifier
		begin	/* If qualifier doesn't match current database */
			raiserror 20001 'Table qualifier must be name of current database'
			return
		end
    end
	if @table_owner is null /*	If owner not supplied, match all */
		select @table_owner = '%'
	if @@trancount <> 0
	begin	/* If inside a transaction */
		raiserror 20003 'The procedure ''sp_pkeys'' cannot be executed from within a transaction.'
		return
    end

	create table #pkeys(
			 TABLE_QUALIFIER varchar(32) NULL,
			 TABLE_OWNER	 varchar(32) NULL,
			 TABLE_NAME 	 varchar(32) NOT NULL,
			 COLUMN_NAME	 varchar(32) NOT NULL,
			 KEY_SEQ		 smallint NOT NULL)

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
		TABLE_QUALIFIER,
		TABLE_OWNER,
		TABLE_NAME,
		COLUMN_NAME,
		KEY_SEQ,
		PK_NAME = convert(varchar(32),null)
	from #pkeys
	where TABLE_NAME = @table_name
		and TABLE_OWNER like @table_owner
	order by 1, 2, 3, 5
go

if (charindex('6.00', @@version) = 0 and
	charindex('6.50', @@version) = 0 and
	charindex('7.00', @@version) = 0)
begin
	print ''
	print ''
	print 'Warning:'
	print 'you are installing the stored procedures '
	print 'on a pre 6.0 SQL Server.'
	print 'Ignore the following error.'
end
else
	drop proc sp_pkeys
go

/*	Procedure for 6.0 server */
CREATE PROCEDURE sp_pkeys(
			   @table_name		varchar(32),
			   @table_owner 	varchar(32) = null,
			   @table_qualifier varchar(32) = null )
as
	DECLARE @table_id			int
	DECLARE @full_table_name	char(70)

	if @table_qualifier is not null
    begin
		if db_name() <> @table_qualifier
		begin	/* If qualifier doesn't match current database */
			raiserror (15250, -1,-1,'Table')
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

    select
		TABLE_QUALIFIER = convert(varchar(32),db_name()),
		TABLE_OWNER = convert(varchar(32),user_name(o.uid)),
		TABLE_NAME = convert(varchar(32),o.name),
		COLUMN_NAME = convert(varchar(32),c.name),
		KEY_SEQ = convert(smallint,c1.colid),
		PK_NAME = convert(varchar(32),i.name)
	from
		sysindexes i, syscolumns c, sysobjects o, syscolumns c1
	where
		o.id = @table_id
		and o.id = c.id
		and o.id = i.id
		and (i.status & 0x800) = 0x800
		and c.name = index_col (@full_table_name, i.indid, c1.colid)
		and c1.colid <= i.keycnt	/* create rows from 1 to keycnt */
		and c1.id = @table_id
	order by 1, 2, 3, 5
go

grant execute on sp_pkeys to public
go

dump tran master with no_log
go

print 'creating sp_server_info'
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

dump tran master with no_log
go

print 'creating sp_special_columns'
go

/*	Procedure for pre-6.0 server */
CREATE PROCEDURE sp_special_columns (
				 @table_name		varchar(32),
				 @table_owner		varchar(32) = null,
				 @table_qualifier	varchar(32) = null,
				 @col_type			char(1) = 'R',
				 @scope				char(1) = 'T',
				 @nullable			char(1) = 'U',
				 @ODBCVer			int = 2)
AS
	DECLARE @indid				int
	DECLARE @table_id			int
	DECLARE @full_table_name	char(70)
	DECLARE @msg				char(70)
	DECLARE @scopeout			smallint

	if @col_type not in ('R','V')
		begin
			raiserror 20002 'Illegal ''col_type'' specified -- must be ''R'' or ''V''.'
			return
		end

	if @scope = 'C'
		select @scopeout = 0
	else if @scope = 'T'
		select @scopeout = 1
	else
		begin
			raiserror 20002 'Illegal ''scope'' specified -- must be ''C'' or ''T''.'
			return
		end

	if @nullable not in ('U','O')
		begin
			raiserror 20002 'Illegal ''nullable'' specified -- must be ''U'' or ''O''.'
			return
		end

	if @table_qualifier is not null
	   begin
		  if db_name() <> @table_qualifier
		      begin	/* If qualifier doesn't match current database */
				raiserror 20001 'Table qualifier must be name of current database'
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
			SCOPE = convert(smallint,NULL),
			COLUMN_NAME = convert(varchar(32),c.name),
			DATA_TYPE = convert(smallint, -3),
			TYPE_NAME = t.name,
			"PRECISION" = convert(int,8),
			LENGTH = convert(int,8),
			SCALE = convert(smallint, NULL),
			PSEUDO_COLUMN = convert(smallint,1)
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
		SCOPE = @scopeout,
		COLUMN_NAME = convert(varchar(32),INDEX_COL(@full_table_name,indid,c2.colid)),
		d.DATA_TYPE,
		TYPE_NAME = t.name,
		"PRECISION" = isnull(d.data_precision, convert(int,c.length)),
		LENGTH = isnull(d.length, convert(int,c.length)),
		SCALE = d.numeric_scale,
		PSEUDO_COLUMN = convert(smallint,1)
	FROM
		sysindexes x,
		syscolumns c,
		master.dbo.spt_datatype_info d,
		systypes t,
		syscolumns c2	/* Self-join to generate list of index columns and */
						/* to extract datatype names */
	WHERE
		x.id = @table_id
		AND c.name = INDEX_COL(@full_table_name,@indid,c2.colid)
		AND c.id = x.id
		AND c2.id = x.id
		AND c2.colid < keycnt+(x.status&16)/16
		AND x.indid = @indid
		AND t.type = d.ss_dtype
		AND c.length = d.fixlen
		AND c.usertype = t.usertype

go

if (charindex('6.00', @@version) = 0 and
	charindex('6.50', @@version) = 0 and
	charindex('7.00', @@version) = 0)
begin
	print ''
	print ''
	print 'Warning:'
	print 'you are installing the stored procedures '
	print 'on a pre 6.0 SQL Server.'
	print 'Ignore the following errors.'
end
else
	drop proc sp_special_columns
go

/*	Procedure for 6.0 server */
CREATE PROCEDURE sp_special_columns (
				 @table_name		varchar(32),
				 @table_owner		varchar(32) = null,
				 @table_qualifier	varchar(32) = null,
				 @col_type			char(1) = 'R',
				 @scope				char(1) = 'T',
				 @nullable			char(1) = 'U',
				 @ODBCVer			int = 2)
AS
	DECLARE @indid				int
	DECLARE @table_id			int
	DECLARE @full_table_name	char(70)
	DECLARE @msg				char(70)
	DECLARE @scopeout			smallint

	if @col_type not in ('R','V')
		begin
			raiserror (15251,-1,-1,'col_type','''R'' or ''V''')
			return
		end

	if @scope = 'C'
		select @scopeout = 0
	else if @scope = 'T'
		select @scopeout = 1
	else
		begin
			raiserror (15251,-1,-1,'scope','''C'' or ''T''')
			return
		end

	if @nullable not in ('U','O')
		begin
			raiserror (15251,-1,-1,'nullable','''U'' or ''O''')
			return
		end

	if @table_qualifier is not null
	   begin
		  if db_name() <> @table_qualifier
		      begin	/* If qualifier doesn't match current database */
				raiserror (15250, -1,-1,'Table')
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
			SCOPE = convert(smallint,NULL),
			COLUMN_NAME = convert(varchar(32),c.name),
			DATA_TYPE = convert(smallint, -2),
			TYPE_NAME = t.name,
			"PRECISION" = convert(int,8),
			LENGTH = convert(int,8),
			SCALE = convert(smallint, NULL),
			PSEUDO_COLUMN = convert(smallint,1)
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
		SCOPE = @scopeout,
		COLUMN_NAME = convert(varchar(32),INDEX_COL(@full_table_name,indid,c2.colid)),
		d.DATA_TYPE,
		convert(varchar(30),case
			when t.usertype > 100 or t.usertype in (18,80) then t.name
			else d.TYPE_NAME
		end) TYPE_NAME,
		convert(int,case
			when d.DATA_TYPE in (6,7) then d.data_precision 		/* FLOAT/REAL */
			else isnull(convert(int,c.prec), 2147483647)
		end) "PRECISION",
		convert(int,case
			when d.ss_dtype IN (106, 108, 55, 63) then	/* decimal/numeric types */
				convert(int,c.prec+2)
			else
				isnull(d.length, c.length)
		end) LENGTH,
		SCALE = convert(smallint, c.scale),
		PSEUDO_COLUMN = convert(smallint,1)
	FROM
		sysindexes x,
		syscolumns c,
		master.dbo.spt_datatype_info d,
		systypes t,
		syscolumns c2	/* Self-join to generate list of index columns and */
						/* to extract datatype names */
	WHERE
		x.id = @table_id
		AND c.name = INDEX_COL(@full_table_name,@indid,c2.colid)
		AND c.id = x.id
		AND c2.id = x.id
		AND c2.colid < x.keycnt+(x.status&16)/16
		AND x.indid = @indid
		AND t.type = d.ss_dtype
		AND (d.ODBCVer is null or d.ODBCVer = @ODBCVer)
		AND isnull(d.AUTO_INCREMENT,0) = (c.status&128)/128
		AND c.usertype = t.usertype
go

grant execute on sp_special_columns to public
go

dump tran master with no_log
go

print 'creating sp_sproc_columns'
go

/*	Procedure for pre-6.0 server */
CREATE PROCEDURE sp_sproc_columns (
				 @procedure_name		varchar(96) = '%',
				 @procedure_owner		varchar(90) = null,
				 @procedure_qualifier	varchar(32) = null,
				 @column_name			varchar(90) = null,
				 @ODBCVer				int = 2)
AS
    DECLARE @group_num int
    DECLARE @semi_position int
	DECLARE @full_procedure_name	char(187)
    DECLARE @procedure_id int

	if @column_name is null /*	If column name not supplied, match all */
		select @column_name = '%'
	if @procedure_qualifier is not null
    begin
		if db_name() <> @procedure_qualifier
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
				raiserror 20001 'Procedure qualifier must be name of current database'
				return
			end
		end
    end

	if @procedure_name is null
	begin	/*	If procedure name not supplied, match all */
		select @procedure_name = '%'
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
		@procedure_id <> 0)
    begin
		/* this block is for the case where there is no pattern
			matching required for the procedure name */
		SELECT
			PROCEDURE_QUALIFIER = convert(varchar(32),DB_NAME()),
			PROCEDURE_OWNER = convert(varchar(32),USER_NAME(o.uid)),
			PROCEDURE_NAME = convert(varchar(41),o.name +';'+ ltrim(str(c.number,5))),
			COLUMN_NAME = convert(varchar(32),c.name),
			COLUMN_TYPE = convert(smallint, 0),
			d.DATA_TYPE,
			TYPE_NAME = t.name,
			"PRECISION" = isnull(d.data_precision, convert(int,c.length)),
			LENGTH = isnull(d.length, convert(int,c.length)),
			SCALE = d.numeric_scale,
			d.RADIX,
			d.NULLABLE,
			REMARKS = convert(varchar(254),null),	/* Remarks are NULL */
			COLUMN_DEF = convert(varchar(254),null),
			d.SQL_DATA_TYPE,
			d.SQL_DATETIME_SUB,
			CHAR_OCTET_LENGTH = isnull(d.data_precision, convert(int,c.length))+d.charbin,
			ORDINAL_POSITION = convert(int,c.colid),
			IS_NULLABLE = convert(varchar(254),rtrim(substring('NO YES',d.NULLABLE*3+1,3))),
			SS_DATA_TYPE = c.type
		FROM
			syscolumns c,
			sysobjects o,
			master.dbo.spt_datatype_info d,
			systypes t
		WHERE
			o.id = @procedure_id
			AND c.id = o.id
			AND t.type = d.ss_dtype
			AND c.length = isnull(d.fixlen, c.length)
			AND (d.ODBCVer is null or d.ODBCVer = @ODBCVer)
			AND c.usertype = t.usertype
			AND c.name like @column_name
			AND c.number = @group_num
		UNION ALL
		SELECT		   /* return value row*/
			PROCEDURE_QUALIFIER = convert(varchar(32),DB_NAME()),
			PROCEDURE_OWNER = convert(varchar(32),USER_NAME(o.uid)),
			PROCEDURE_NAME = convert(varchar(41),o.name +';'+ isnull(ltrim(str(c.number,5)),'1')),
			COLUMN_NAME = convert(varchar(32),'RETURN_VALUE'),
			COLUMN_TYPE = convert(smallint, 5),
			DATA_TYPE = convert(smallint, 4),
			TYPE_NAME = convert(varchar(30),'int'),
			"PRECISION" = convert(int,10),
			LENGTH = convert(int,4),
			SCALE = convert(smallint,0),
			RADIX = convert(smallint,10),
			NULLABLE = convert(smallint,0),
			REMARKS = convert(varchar(254),null),	/* Remarks are NULL */
			COLUMN_DEF = convert(varchar(254),NULL),
			SQL_DATA_TYPE = convert(smallint, 4),
			SQL_DATETIME_SUB = convert(smallint,null),
			CHAR_OCTET_LENGTH = convert(int,null),
			ORDINAL_POSITION = convert(int,0),
			IS_NULLABLE = convert(varchar(254),'NO'),
			SS_DATA_TYPE = convert(tinyint,56)
		FROM
			syscolumns c,
			sysobjects o
		WHERE
			o.id = @procedure_id
			AND c.id =* o.id
			AND c.colid = 1
			AND 'RETURN_VALUE' like @column_name
		ORDER BY 1, 2, 3, 18
	end
	else
    begin
		/* this block is for the case where there IS pattern
			matching done on the procedure name */
		if @procedure_owner is null
			select @procedure_owner = '%'
		SELECT
			PROCEDURE_QUALIFIER = convert(varchar(32),DB_NAME()),
			PROCEDURE_OWNER = convert(varchar(32),USER_NAME(o.uid)),
			PROCEDURE_NAME = convert(varchar(41),o.name +';'+ ltrim(str(c.number,5))),
			COLUMN_NAME = convert(varchar(32),c.name),
			COLUMN_TYPE = convert(smallint, 0),
			d.DATA_TYPE,
			TYPE_NAME = t.name,
			"PRECISION" = isnull(d.data_precision, convert(int,c.length)),
			LENGTH = isnull(d.length, convert(int,c.length)),
			SCALE = d.numeric_scale,
			d.RADIX,
			d.NULLABLE,
			REMARKS = convert(varchar(254),null),	/* Remarks are NULL */
			COLUMN_DEF = convert(varchar(254),null),
			d.SQL_DATA_TYPE,
			d.SQL_DATETIME_SUB,
			CHAR_OCTET_LENGTH = isnull(d.data_precision, convert(int,c.length))+d.charbin,
			ORDINAL_POSITION = convert(int,c.colid),
			IS_NULLABLE = convert(varchar(254),rtrim(substring('NO YES',d.NULLABLE*3+1,3))),
			SS_DATA_TYPE = c.type
		FROM
			syscolumns c,
			sysobjects o,
			master.dbo.spt_datatype_info d,
			systypes t
		WHERE
			o.name like @procedure_name
			AND user_name(o.uid) like @procedure_owner
			AND o.id = c.id
			AND t.type = d.ss_dtype
			AND c.length = isnull(d.fixlen, c.length)
			AND (d.ODBCVer is null or d.ODBCVer = @ODBCVer)
			AND c.usertype = t.usertype
			AND o.type = 'P'							/* Just Procedures */
			AND c.name like @column_name
		UNION ALL
		SELECT		   /* return value row*/
			PROCEDURE_QUALIFIER = convert(varchar(32),DB_NAME()),
			PROCEDURE_OWNER = convert(varchar(32),USER_NAME(o.uid)),
			PROCEDURE_NAME = convert(varchar(41),o.name +';'+ isnull(ltrim(str(c.number,5)),'1')),
			COLUMN_NAME = convert(varchar(32),'RETURN_VALUE'),
			COLUMN_TYPE = convert(smallint, 5),
			DATA_TYPE = convert(smallint, 4),
			TYPE_NAME = convert(varchar(30),'int'),
			"PRECISION" = convert(int,10),
			LENGTH = convert(int,4),
			SCALE = convert(smallint,0),
			RADIX = convert(smallint,10),
			NULLABLE = convert(smallint,0),
			REMARKS = convert(varchar(254),null),	/* Remarks are NULL */
			COLUMN_DEF = convert(varchar(254),NULL),
			SQL_DATA_TYPE = convert(smallint, 4),
			SQL_DATETIME_SUB = convert(smallint,null),
			CHAR_OCTET_LENGTH = convert(int,null),
			ORDINAL_POSITION = convert(int,0),
			IS_NULLABLE = convert(varchar(254),'NO'),
			SS_DATA_TYPE = convert(tinyint,56)
		FROM
			syscolumns c,
			sysobjects o
		WHERE
			o.name like @procedure_name
			AND user_name(o.uid) like @procedure_owner
			AND c.id =* o.id
			AND c.colid = 1
			AND o.type = 'P'						/* Just Procedures */
			AND 'RETURN_VALUE' like @column_name
		ORDER BY 1, 2, 3, 18
	end
go

if (charindex('6.00', @@version) = 0 and
	charindex('6.50', @@version) = 0 and
	charindex('7.00', @@version) = 0)
begin
	print ''
	print ''
	print 'Warning:'
	print 'you are installing the stored procedures '
	print 'on a pre 6.0 SQL Server.'
	print 'Ignore the following error.'
end
else
	drop proc sp_sproc_columns
go

/*	Procedure for 6.0 server */
CREATE PROCEDURE sp_sproc_columns (
				 @procedure_name		varchar(96) = '%',
				 @procedure_owner		varchar(90) = null,
				 @procedure_qualifier	varchar(32) = null,
				 @column_name			varchar(90) = null,
				 @ODBCVer				int = 2)
AS
    DECLARE @group_num int
    DECLARE @semi_position int
	DECLARE @full_procedure_name	char(187)
    DECLARE @procedure_id int

	if @column_name is null /*	If column name not supplied, match all */
		select @column_name = '%'
	if @procedure_qualifier is not null
    begin
		if db_name() <> @procedure_qualifier
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
				raiserror (15250, -1,-1,'Procedure')
				return
			end
		end
    end

	if @procedure_name is null
	begin	/*	If procedure name not supplied, match all */
		select @procedure_name = '%'
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
		@procedure_id <> 0)
    begin
		/* this block is for the case where there is no pattern
			matching required for the procedure name */
		SELECT
			PROCEDURE_QUALIFIER = convert(varchar(32),DB_NAME()),
			PROCEDURE_OWNER = convert(varchar(32),USER_NAME(o.uid)),
			PROCEDURE_NAME = convert(varchar(41),o.name +';'+ ltrim(str(c.number,5))),
			COLUMN_NAME = convert(varchar(32),c.name),
			COLUMN_TYPE = convert(smallint, 1+((c.status/64)&1)),
			d.DATA_TYPE,
			TYPE_NAME = t.name,
			convert(int,case
				when d.DATA_TYPE in (6,7) then d.data_precision 		/* FLOAT/REAL */
				else isnull(convert(int,c.prec), 2147483647)
			end) "PRECISION",
			convert(int,case
				when d.ss_dtype IN (106, 108, 55, 63) then	/* decimal/numeric types */
					c.prec+2
				else
					isnull(d.length, c.length)
			end) LENGTH,
			SCALE = convert(smallint, c.scale),
			d.RADIX,
			d.NULLABLE,
			REMARKS = convert(varchar(254),null),	/* Remarks are NULL */
			COLUMN_DEF = convert(varchar(254),NULL),
			d.SQL_DATA_TYPE,
			d.SQL_DATETIME_SUB,
			CHAR_OCTET_LENGTH = isnull(convert(int,c.prec), 2147483647)+d.charbin,
			ORDINAL_POSITION = convert(int,c.colid),
			IS_NULLABLE = convert(varchar(254),rtrim(substring('NO YES',d.NULLABLE*3+1,3))),
			SS_DATA_TYPE = c.type
		FROM
			syscolumns c,
			sysobjects o,
			master.dbo.spt_datatype_info d,
			systypes t
		WHERE
			o.id = @procedure_id
			AND c.id = o.id
			AND c.type = d.ss_dtype
			AND c.length = isnull(d.fixlen, c.length)
			AND (d.ODBCVer is null or d.ODBCVer = @ODBCVer)
			AND isnull(d.AUTO_INCREMENT,0) = 0
			AND c.usertype *= t.usertype
			AND c.name like @column_name
			AND c.number = @group_num
		UNION ALL
		SELECT		   /* return value row*/
			PROCEDURE_QUALIFIER = convert(varchar(32),DB_NAME()),
			PROCEDURE_OWNER = convert(varchar(32),USER_NAME(o.uid)),
			PROCEDURE_NAME = convert(varchar(41),o.name +';'+ isnull(ltrim(str(c.number,5)),'1')),
			COLUMN_NAME = convert(varchar(32),'RETURN_VALUE'),
			COLUMN_TYPE = convert(smallint, 5),
			DATA_TYPE = convert(smallint, 4),
			TYPE_NAME = convert(varchar(30),'int'),
			"PRECISION" = convert(int,10),
			LENGTH = convert(int,4),
			SCALE = convert(smallint,0),
			RADIX = convert(smallint,10),
			NULLABLE = convert(smallint,0),
			REMARKS = convert(varchar(254),null),	/* Remarks are NULL */
			COLUMN_DEF = convert(varchar(254),NULL),
			SQL_DATA_TYPE = convert(smallint, 4),
			SQL_DATETIME_SUB = convert(smallint,null),
			CHAR_OCTET_LENGTH = convert(int,null),
			ORDINAL_POSITION = convert(int,0),
			IS_NULLABLE = convert(varchar(254),'NO'),
			SS_DATA_TYPE = convert(tinyint,56)
		FROM
			syscolumns c,
			sysobjects o
		WHERE
			o.id = @procedure_id
			AND c.id =* o.id
			AND c.colid = 1
			AND 'RETURN_VALUE' like @column_name
		ORDER BY 1, 2, 3, 18
	end
	else
    begin
		/* this block is for the case where there IS pattern
			matching done on the procedure name */
		if @procedure_owner is null
			select @procedure_owner = '%'
		SELECT
			PROCEDURE_QUALIFIER = convert(varchar(32),DB_NAME()),
			PROCEDURE_OWNER = convert(varchar(32),USER_NAME(o.uid)),
			PROCEDURE_NAME = convert(varchar(41),o.name +';'+ ltrim(str(c.number,5))),
			COLUMN_NAME = convert(varchar(32),c.name),
			COLUMN_TYPE = convert(smallint, 1+((c.status/64)&1)),
			d.DATA_TYPE,
			TYPE_NAME = t.name,
			convert(int,case
				when d.DATA_TYPE in (6,7) then d.data_precision 		/* FLOAT/REAL */
				else isnull(convert(int,c.prec), 2147483647)
			end) "PRECISION",
			convert(int,case
				when d.ss_dtype IN (106, 108, 55, 63) then	/* decimal/numeric types */
					c.prec+2
				else
					isnull(d.length, c.length)
			end) LENGTH,
			SCALE = convert(smallint, c.scale),
			d.RADIX,
			d.NULLABLE,
			REMARKS = convert(varchar(254),null),	/* Remarks are NULL */
			COLUMN_DEF = convert(varchar(254),NULL),
			d.SQL_DATA_TYPE,
			d.SQL_DATETIME_SUB,
			CHAR_OCTET_LENGTH = isnull(convert(int,c.prec), 2147483647)+d.charbin,
			ORDINAL_POSITION = convert(int,c.colid),
			IS_NULLABLE = convert(varchar(254),rtrim(substring('NO YES',d.NULLABLE*3+1,3))),
			SS_DATA_TYPE = c.type
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
			AND c.length = isnull(d.fixlen, c.length)
			AND (d.ODBCVer is null or d.ODBCVer = @ODBCVer)
			AND isnull(d.AUTO_INCREMENT,0) = 0
			AND c.usertype *= t.usertype
			AND o.type = 'P'							/* Just Procedures */
			AND c.name like @column_name
		UNION ALL
		SELECT		   /* return value row*/
			PROCEDURE_QUALIFIER = convert(varchar(32),DB_NAME()),
			PROCEDURE_OWNER = convert(varchar(32),USER_NAME(o.uid)),
			PROCEDURE_NAME = convert(varchar(41),o.name +';'+ isnull(ltrim(str(c.number,5)),'1')),
			COLUMN_NAME = convert(varchar(32),'RETURN_VALUE'),
			COLUMN_TYPE = convert(smallint, 5),
			DATA_TYPE = convert(smallint, 4),
			TYPE_NAME = convert(varchar(30),'int'),
			"PRECISION" = convert(int,10),
			LENGTH = convert(int,4),
			SCALE = convert(smallint,0),
			RADIX = convert(smallint,10),
			NULLABLE = convert(smallint,0),
			REMARKS = convert(varchar(254),null),	/* Remarks are NULL */
			COLUMN_DEF = convert(varchar(254),NULL),
			SQL_DATA_TYPE = convert(smallint, 4),
			SQL_DATETIME_SUB = convert(smallint,null),
			CHAR_OCTET_LENGTH = convert(int,null),
			ORDINAL_POSITION = convert(int,0),
			IS_NULLABLE = convert(varchar(254),'NO'),
			SS_DATA_TYPE = convert(tinyint,56)
		FROM
			syscolumns c,
			sysobjects o
		WHERE
			o.name like @procedure_name
			AND user_name(o.uid) like @procedure_owner
			AND c.id =* o.id
			AND c.colid = 1
			AND o.type = 'P'						/* Just Procedures */
			AND 'RETURN_VALUE' like @column_name
		ORDER BY 1, 2, 3, 18
	end
go

grant execute on sp_sproc_columns to public
go

dump tran master with no_log
go

print 'creating sp_statistics'
go

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
	DECLARE @full_table_name	char(70)

	if @table_qualifier is not null
    begin
		if db_name() <> @table_qualifier
		begin	/* If qualifier doesn't match current database */
			raiserror 20001 'Table qualifier must be name of current database'
			return
		end
    end

	if @accuracy not in ('Q','E')
		begin
			raiserror 20002 'Illegal ''accuracy'' specified -- must be ''Q'' or ''E''.'
			return
		end

	if (@@trancount <> 0 and
		charindex('6.50', @@version) = 0 and
		charindex('7.00', @@version) = 0)
	begin	/* If inside a transaction */
		raiserror 20003 'The procedure ''sp_statistics'' cannot be executed from within a transaction.'
		return
    end
	create table #TmpIndex(
		TABLE_QUALIFIER varchar(32) NULL,
		TABLE_OWNER 	varchar(32) NULL,
		TABLE_NAME		varchar(32) NOT NULL,
		INDEX_QUALIFIER varchar(32) null,
		INDEX_NAME		varchar(32) null,
		NON_UNIQUE		smallint null,
		TYPE			smallint NOT NULL,
		SEQ_IN_INDEX	smallint null,
		COLUMN_NAME 	varchar(32) null,
		COLLATION		char(1) null,
		index_id		int null,
		CARDINALITY 	int null,
		PAGES			int null,
		status			smallint NOT NULL)
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

	WHILE @indid <> NULL
    BEGIN
		INSERT #TmpIndex	/* Add all columns that are in index */
			SELECT
				DB_NAME(),								/* TABLE_QUALIFIER */
				USER_NAME(o.uid),						/* TABLE_OWNER	   */
				o.name, 								/* TABLE_NAME	   */
				o.name, 								/* INDEX_QUALIFIER */
				x.name, 								/* INDEX_NAME	   */
				0,										/* NON_UNIQUE	   */
				1,										/* SQL_INDEX_CLUSTERED */
				colid,									/* SEQ_IN_INDEX    */
				INDEX_COL(@full_table_name,indid,colid),/* COLUMN_NAME	   */
				'A',									/* COLLATION	   */
				@indid, 								/* index_id 	   */
				x.rows, 								/* CARDINALITY	   */
				x.dpages,								/* PAGES		   */
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
		SET NON_UNIQUE = 1
		WHERE status&2 <> 2 /* If non-unique index */
    UPDATE #TmpIndex
		SET
			TYPE = 3,			/* SQL_INDEX_OTHER */
			CARDINALITY = NULL,
			PAGES = NULL
		WHERE index_id > 1	/* If non-clustered index */

	/* now add row for table statistics */
	INSERT #TmpIndex
		SELECT
			DB_NAME(),				/* TABLE_QUALIFIER */
			USER_NAME(o.uid),		/* TABLE_OWNER	   */
			o.name, 				/* TABLE_NAME	   */
			null,					/* INDEX_QUALIFIER */
			null,					/* INDEX_NAME	   */
			null,					/* NON_UNIQUE	   */
			0,						/* SQL_TABLE_STAT  */
			null,					/* SEQ_IN_INDEX    */
			null,					/* COLUMN_NAME	   */
			null,					/* COLLATION	   */
			0,						/* index_id 	   */
			x.rows, 				/* CARDINALITY	   */
			x.dpages,				/* PAGES		   */
			0						/* status		   */
		FROM sysindexes x, sysobjects o
		WHERE o.id = @table_id
			AND x.id = o.id
			AND (x.indid = 0 or x.indid = 1)	/*	If there are no indexes */
												/*	then table stats are in */
												/*	a row with indid =0		*/

	if @is_unique <> 'Y'	/* If all indexes desired */
		SELECT
			TABLE_QUALIFIER,
			TABLE_OWNER,
			TABLE_NAME,
			NON_UNIQUE,
			INDEX_QUALIFIER,
			INDEX_NAME,
			TYPE,
			SEQ_IN_INDEX,
			COLUMN_NAME,
			COLLATION,
			CARDINALITY,
			PAGES,
			FILTER_CONDITION = convert(varchar(128),null)
		FROM #TmpIndex
		WHERE
			INDEX_NAME like @index_name /* If matching name */
			or INDEX_NAME is null		/* If SQL_TABLE_STAT row */
		ORDER BY 4, 7, 6, 8
	else					/* If only unique indexes desired */
		SELECT
			TABLE_QUALIFIER,
			TABLE_OWNER,
			TABLE_NAME,
			NON_UNIQUE,
			INDEX_QUALIFIER,
			INDEX_NAME,
			TYPE,
			SEQ_IN_INDEX,
			COLUMN_NAME,
			COLLATION,
			CARDINALITY,
			PAGES,
			FILTER_CONDITION = convert(varchar(128),null)
		FROM #TmpIndex
		WHERE
			(NON_UNIQUE = 0 			/* If unique */
				or NON_UNIQUE is NULL)	/* If SQL_TABLE_STAT row */
			and (INDEX_NAME like @index_name	/* If matching name */
				or INDEX_NAME is null)	/* If SQL_TABLE_STAT row */
		ORDER BY 4, 7, 6, 8

    DROP TABLE #TmpIndex
go

grant execute on sp_statistics to public
go

dump tran master with no_log
go

print 'creating sp_stored_procedures'
go

create procedure sp_stored_procedures(
						@sp_name		varchar(96) = null,
						@sp_owner		varchar(90) = null,
						@sp_qualifier	varchar(32) = null)
as
	declare @proc_type smallint

    if @sp_qualifier is not null
    begin
		if db_name() <> @sp_qualifier
		begin
			if @sp_qualifier = ''
			begin
				/* in this case, we need to return an empty result set */
				/* because the user has requested a database with an empty name */
				select @sp_name = ''
				select @sp_owner = ''
			end else
			begin	/* If qualifier doesn't match current database */
				raiserror 20001 'Procedure qualifier must be name of current database'
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

	select @proc_type=2		/* Return 2 for 4.2 and later servers. */

    select
		PROCEDURE_QUALIFIER = convert(varchar(32),db_name()),
		PROCEDURE_OWNER = convert(varchar(32),user_name(o.uid)),
		PROCEDURE_NAME = convert(varchar(41),o.name +';'+ ltrim(str(p.number,5))),
		NUM_INPUT_PARAMS = -1,	/* Constant since value unknown */
		NUM_OUTPUT_PARAMS = -1, /* Constant since value unknown */
		NUM_RESULT_SETS = -1,	/* Constant since value unknown */
		REMARKS = convert(varchar(254),null),	/* Remarks are NULL */
		PROCEDURE_TYPE = @proc_type
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
	order by 1, 2, 3
go
grant execute on sp_stored_procedures to public
go

dump tran master with no_log
go


print 'creating sp_table_privileges'
go

/*	Procedure for pre 6.50 server */
CREATE PROCEDURE sp_table_privileges (
			@table_name 		varchar(90),
			@table_owner		varchar(90) = null,
			@table_qualifier	varchar(32) = null)
as

	if @table_qualifier is not null
    begin
		if db_name() <> @table_qualifier
		begin	/* If qualifier doesn't match current database */
			raiserror 20001 'Table qualifier must be name of current database'
			return
		end
    end
	if (@@trancount <> 0)
	begin	/* If inside a transaction */
		raiserror 20003 'The procedure ''sp_table_privileges'' cannot be executed from within a transaction.'
		return
    end
	if @table_name is null
		select @table_name = '%'
	if @table_owner is null /* If no owner supplied, force wildcard */
		select @table_owner = '%'

	create table #table_priv1(
		id						int NOT NULL,
		grantor 				smallint NOT NULL,
		grantee 				smallint NOT NULL,
		select_privilege		bit,
		insert_privilege		bit,
		update_privilege		bit,
		delete_privilege		bit,
		references_privilege	bit,
		select_privilege_grant	bit,
		insert_privilege_grant	bit,
		update_privilege_grant	bit,
		delete_privilege_grant	bit,
		references_privilege_grant	bit,
		uid 					smallint NOT NULL,
		gid 					smallint NOT NULL)

    insert into #table_priv1
		select distinct
			o.id,
			o.uid,
			u.uid,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			u.uid,
			u.gid
		from sysusers u, sysobjects o, sysprotects p
		where
			o.name like @table_name
			and user_name(o.uid) like @table_owner
			and u.uid <> u.gid
			and u.uid *= p.uid and o.id *= p.id
			and p.protecttype in (204, 205)
			and sysstat & 0xf in (1,2,3)	/* only valid for system tables,
											** user tables, and views. */

    /*
	** now add/update row for table owner
    */
    if exists (
		select *
			from #table_priv1
			where grantor = grantee)
    begin
		update #table_priv1
		set
			select_privilege_grant = 1,
			update_privilege_grant = 1,
			insert_privilege_grant = 1,
			delete_privilege_grant = 1,
			references_privilege_grant = 1,
			grantor = (select uid from sysusers where suid = 1)
		where grantor = grantee
    end
    else
    begin
		insert into #table_priv1
			select
				o.id,
				u1.uid,
				o.uid,
				0,
				0,
				0,
				0,
				0,
				1,
				1,
				1,
				1,
				1,
				o.uid,
				u.gid
			from sysobjects o, sysusers u, sysusers u1
			where o.name like @table_name
			and user_name(o.uid) like @table_owner
			and u.uid = o.uid
			and sysstat & 0xf in (1,2,3)	/* only valid for system tables,
		   								** user tables, and views. */
			and u1.suid = 1		/* grantor is dbo of database */

    end

    update #table_priv1
	set select_privilege = 1
		from sysprotects p
	where
		#table_priv1.id = p.id
		and (#table_priv1.uid = p.uid
			or #table_priv1.gid = p.uid
			or p.uid = 0)
		and protecttype = 205
		and action = 193
		and not exists (
			select * from sysprotects
			where
				#table_priv1.id = sysprotects.id
				and (#table_priv1.uid = uid
					or #table_priv1.gid = uid
					or uid = 0)
				and protecttype = 206
				and action = 193)

    update #table_priv1
	set insert_privilege = 1
		from sysprotects p
	where
		#table_priv1.id = p.id
		and (#table_priv1.uid = p.uid
			or #table_priv1.gid = p.uid
			or p.uid = 0)
		and protecttype = 205
		and action = 195
		and not exists (
			select * from sysprotects
			where
				#table_priv1.id = sysprotects.id
				and (#table_priv1.uid = uid
					or #table_priv1.gid = uid
					or uid = 0)
				and protecttype = 206
				and action = 195)

    update #table_priv1
	set delete_privilege = 1
		from sysprotects p
	where
		exists (
			select * from sysprotects
			where
				#table_priv1.id = sysprotects.id
				and (#table_priv1.uid = uid
					or #table_priv1.gid = uid
					or uid = 0)
				and protecttype = 205
				and action = 196)
		and not exists (select * from sysprotects
			where
				#table_priv1.id = sysprotects.id
				and (#table_priv1.uid = uid
					or #table_priv1.gid = uid
					or uid = 0)
				and protecttype = 206
				and action = 196)

    update #table_priv1
	set update_privilege = 1
		from sysprotects p
	where
		#table_priv1.id = p.id
		and (#table_priv1.uid = p.uid
			or #table_priv1.gid = p.uid
			or p.uid = 0)
		and protecttype = 205
		and action = 197
		and not exists (
			select * from sysprotects
			where
				#table_priv1.id = sysprotects.id
				and (#table_priv1.uid = uid
					or #table_priv1.gid = uid
					or uid = 0)
				and protecttype = 206
				and action = 197)

    update #table_priv1
	set references_privilege = 1
		from sysprotects p
	where
		#table_priv1.id = p.id
		and (#table_priv1.uid = p.uid
			or #table_priv1.gid = p.uid
			or p.uid = 0)
		and protecttype = 205
		and action = 26
		and not exists (
			select * from sysprotects
			where
				#table_priv1.id = sysprotects.id
				and (#table_priv1.uid = uid
					or #table_priv1.gid = uid
					or uid = 0)
				and protecttype = 206
				and action = 26)

	create table #table_priv2(
		id				int NOT NULL,
		grantor 		smallint NOT NULL,
		grantee 		smallint NOT NULL,
		PRIVILEGE		varchar(32) NOT NULL,
		IS_GRANTABLE	varchar(3) NULL)

	insert into #table_priv2
		select
			id,
			grantor,
			grantee,
			'SELECT',
			'NO'
		from #table_priv1
		where select_privilege = 1 and select_privilege_grant = 0

	insert into #table_priv2
		select
			id,
			grantor,
			grantee,
			'INSERT',
			'NO'
		from #table_priv1
		where insert_privilege = 1 and insert_privilege_grant = 0

	insert into #table_priv2
		select
			id,
			grantor,
			grantee,
			'DELETE',
			'NO'
		from #table_priv1
		where delete_privilege = 1 and delete_privilege_grant = 0

	insert into #table_priv2
		select
			id,
			grantor,
			grantee,
			'UPDATE',
			'NO'
		from #table_priv1
		where update_privilege = 1 and update_privilege_grant = 0

	insert into #table_priv2
		select
			id,
			grantor,
			grantee,
			'REFERENCES',
			'NO'
		from #table_priv1
		where references_privilege = 1 and references_privilege_grant = 0

	insert into #table_priv2
		select
			id,
			grantor,
			grantee,
			'SELECT',
			'YES'
		from #table_priv1
		where select_privilege_grant = 1

	insert into #table_priv2
		select
			id,
			grantor,
			grantee,
			'INSERT',
			'YES'
		from #table_priv1
		where insert_privilege_grant = 1

	insert into #table_priv2
		select
			id,
			grantor,
			grantee,
			'DELETE',
			'YES'
		from #table_priv1
		where delete_privilege_grant = 1

	insert into #table_priv2
		select
			id,
			grantor,
			grantee,
			'UPDATE',
			'YES'
		from #table_priv1
		where update_privilege_grant = 1

	insert into #table_priv2
		select
			id,
			grantor,
			grantee,
			'REFERENCES',
			'YES'
		from #table_priv1
		where references_privilege_grant = 1


	select
		convert(varchar(32),db_name()) TABLE_QUALIFIER,
		convert(varchar(32),user_name(o.uid)) TABLE_OWNER,
		convert(varchar(32),o.name) TABLE_NAME,
		convert(varchar(32),user_name(grantor)) GRANTOR,
		convert(varchar(32),user_name(grantee)) GRANTEE,
		PRIVILEGE,
		IS_GRANTABLE
	from #table_priv2 t, sysobjects o where o.id = t.id
	order by 2,3,6
go

if (charindex('6.50', @@version) = 0 and
	charindex('7.00', @@version) = 0)
begin
	print ''
	print ''
	print 'Warning:'
	print 'you are installing the stored procedures '
	print 'on a pre 6.50 SQL Server.'
	print 'Ignore the following errors.'
end
else
	drop proc sp_table_privileges
go

/*	Procedure for 6.50 server */
CREATE PROCEDURE sp_table_privileges (
			@table_name 		varchar(90),
			@table_owner		varchar(90) = null,
			@table_qualifier	varchar(32) = null)
as

	if @table_qualifier is not null
    begin
		if db_name() <> @table_qualifier
		begin	/* If qualifier doesn't match current database */
			raiserror 20001 'Table qualifier must be name of current database'
			return
		end
    end
	if @table_name is null
		select @table_name = '%'
	if @table_owner is null /* If no owner supplied, force wildcard */
		select @table_owner = '%'

	select
		convert(varchar(32),db_name()) TABLE_QUALIFIER,
		convert(varchar(32),user_name(o.uid)) TABLE_OWNER,
		convert(varchar(32),object_name(o.id)) TABLE_NAME,
		convert(varchar(32),user_name(p.grantor)) GRANTOR,
		convert(varchar(32),user_name(u.uid)) GRANTEE,
		convert(varchar(32),case p.action
			 when 193 then 'SELECT'
			 when 195 then 'INSERT'
			 when 196 then 'DELETE'
			 when 197 then 'UPDATE'
			 else 'REFERENCES'
		end) PRIVILEGE,
		convert(varchar(3),case when p.protecttype = 205 then 'NO'
			else 'YES'
		end) IS_GRANTABLE
	from sysprotects p, sysobjects o, sysusers u
	where
		p.id = o.id
		and object_name(o.id) like @table_name
		and user_name(o.uid) like @table_owner
			/* expand groups */
		and ((p.uid = u.uid and u.uid <> u.gid) or
			 (p.uid = u.gid and u.uid <> u.gid))
		and p.protecttype <> 206	/* only grant rows */
		and p.action in (26,193,195,196,197)
		and o.uid <> u.uid			/* no rows for owner */
		and not exists (			/* exclude revoke'd privileges */
			select *
			from sysprotects p1
			where
				p1.protecttype = 206
				and p1.action = p.action
				and p1.id = p.id
				and p1.uid = u.uid)
	union all
	select	/*	Add rows for table owner */
		convert(varchar(32),db_name()) TABLE_QUALIFIER,
		convert(varchar(32),user_name(o.uid)) TABLE_OWNER,
		convert(varchar(32),object_name(o.id)) TABLE_NAME,
		convert(varchar(32),user_name(u.uid)) GRANTOR,
		convert(varchar(32),user_name(o.uid)) GRANTEE,
		convert(varchar(32),case v.number
			when 193 then 'SELECT'
			when 195 then 'INSERT'
			when 196 then 'DELETE'
			when 197 then 'UPDATE'
			else 'REFERENCES'
		end) PRIVILEGE,
		convert(varchar(3),'YES') IS_GRANTABLE
	from sysobjects o, spt_values v, sysusers u
	where
		object_name(o.id) like @table_name
		and user_name(o.uid) like @table_owner
		and u.suid = 1		/* grantor is dbo of database */
		and v.type = 'P'	/* cross product to get all exposed privileges */
		and v.number in (26,193,195,196,197)
		and not exists (	/* exclude revoke'd privileges */
			select *
			from sysprotects p1
			where
				p1.protecttype = 206
				and p1.action = v.number
				and p1.id = o.id
				and p1.uid = o.uid)
	order by 2,3,6
go

grant execute on sp_table_privileges to public
go

dump tran master with no_log
go

print 'creating sp_tables'
go

create procedure sp_tables(
			   @table_name		varchar(90)	= null,
			   @table_owner 	varchar(90)	= null,
			   @table_qualifier varchar(90)	= null,
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
			TABLE_QUALIFIER = convert(varchar(32),d.name),
			TABLE_OWNER = convert(varchar(32),null),
			TABLE_NAME = convert(varchar(32),null),
			TABLE_TYPE = convert(varchar(32),null),
			REMARKS = convert(varchar(254),null)	/* Remarks are NULL */
		from master..sysdatabases d
		where d.name <> 'model'	/* eliminate MODEL database */
		order by 1
	end

	/* Special feature #2:	enumerate owners when qualifier and name
		 are blank but owner is explicitly '%'.  */
	else if @table_qualifier = '' and
		@table_owner = '%' and
		@table_name = ''
	begin	/* If enumerating owners */
		select distinct
			TABLE_QUALIFIER = convert(varchar(32),null),
			TABLE_OWNER = convert(varchar(32),user_name(uid)),
			TABLE_NAME = convert(varchar(32),null),
			TABLE_TYPE = convert(varchar(32),null),
			REMARKS = convert(varchar(254),null)	/* Remarks are NULL */
		from sysobjects
		order by 2
	end

	/* Special feature #3:	enumerate table types when qualifier, owner and
		 name are blank but table type is explicitly '%'.	*/
	else if @table_qualifier = '' and
		@table_owner = '' and
		@table_name = '' and
		@table_type = '%'
	begin	/* If enumerating table types */
		select
			TABLE_QUALIFIER = convert(varchar(32),null),
			TABLE_OWNER = convert(varchar(32),null),
			TABLE_NAME = convert(varchar(32),null),
			TABLE_TYPE = convert(varchar(32),rtrim(substring('SYSTEM TABLETABLE       VIEW',(colid-1)*12+1,12))),
			REMARKS = convert(varchar(254),null)	/* Remarks are NULL */
		from sysobjects o, syscolumns c
		where o.id=c.id and o.name='sysusers' and colid<=3
	end

	else
	begin /* end of special features -- do normal processing */
		if @table_qualifier is not null
		begin
			if db_name() <> @table_qualifier
			begin
				if @table_qualifier = ''
				begin  /* If empty qualifier supplied */
					/* Force an empty result set */
					select @table_name = ''
					select @table_owner = ''
				end
				else
				begin	/* If qualifier doesn't match current database */
					raiserror 20001 'Table qualifier must be name of current database'
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
			if (charindex('''SYSTEM TABLE''',@table_type) <> 0)
				select @type1 = @type1 + 'S'	/* Add System Tables */
			if (charindex('''TABLE''',@table_type) <> 0)
				select @type1 = @type1 + 'U'	/* Add User Tables */
			if (charindex('''VIEW''',@table_type) <> 0)
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
			TABLE_QUALIFIER = convert(varchar(32),db_name()),
			TABLE_OWNER = convert(varchar(32),user_name(o.uid)),
			TABLE_NAME = convert(varchar(32),o.name),	/* make nullable */
			TABLE_TYPE = convert(varchar(32),rtrim(
				substring('SYSTEM TABLE            TABLE       VIEW       ',
					(ascii(o.type)-83)*12+1,12))),	/* 'S'=0,'U'=2,'V'=3 */
			REMARKS = convert(varchar(254),null)	/* Remarks are NULL */
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
		order by 4, 1, 2, 3
	end
go

grant execute on sp_tables to public
go

dump tran master with no_log
go

if (charindex('6.50', @@version) = 0 and
	charindex('7.00', @@version) = 0)
begin
	print ''
	print ''
	print 'Warning:'
	print 'you are installing the stored procedures '
	print 'on a pre 6.50 SQL Server.'
	print 'Ignore the following errors.'
end

print 'creating sp_ddopen'
go

create procedure sp_ddopen(
			   @handle			int output,
			   @procname		sysname(30),
			   @scrollopt		int output,
			   @ccopt			int output,
			   @rows			int output,
			   @p1				varchar(255) = null,
			   @p2				varchar(255) = null,
			   @p3				varchar(255) = null,
			   @p4				varchar(255) = null,
			   @p5				varchar(255) = null,
			   @p6				varchar(255) = null,
			   @p7				int = null,
			   @ODBCVer 		int = 2)
as
	declare @ret int

	if @procname = 'sp_column_privileges'
	begin
		create table #spcolpriv (
			TABLE_QUALIFIER varchar(32) null,
			TABLE_OWNER varchar(32) null,
			TABLE_NAME varchar(32) not null,
			COLUMN_NAME varchar(32) not null,
			GRANTOR varchar(32) null,
			GRANTEE varchar(32) not null,
			PRIVILEGE varchar(32) not null,
			IS_GRANTABLE varchar(3) null
			)
		insert into #spcolpriv exec sp_column_privileges @p1,@p2,@p3,@p4
		exec @ret = sp_cursoropen @handle output,
			'select * from #spcolpriv',
			@scrollopt output, @ccopt output, @rows output
		drop table #spcolpriv
	end
	else if @procname = 'sp_columns'
	begin
		create table #spcolumns (
			TABLE_QUALIFIER varchar(32) null,
			TABLE_OWNER varchar(32) null,
			TABLE_NAME varchar(32) not null,
			COLUMN_NAME varchar(32) not null,
			DATA_TYPE smallint not null,
			TYPE_NAME varchar(30) not null,
			"PRECISION" int null,
			LENGTH int null,
			SCALE smallint null,
			RADIX smallint null,
			NULLABLE smallint not null,
			REMARKS varchar(254) null,
			COLUMN_DEF varchar(254) null,
			SQL_DATA_TYPE smallint not null,
			SQL_DATETIME_SUB smallint null,
			CHAR_OCTET_LENGTH int null,
			ORDINAL_POSITION int not null,
			IS_NULLABLE varchar(254) null,
			SS_DATA_TYPE tinyint null
			)
		insert into #spcolumns exec sp_columns @p1,@p2,@p3,@p4,@ODBCVer
		exec @ret = sp_cursoropen @handle output,
			'select * from #spcolumns',
			@scrollopt output, @ccopt output, @rows output
		drop table #spcolumns
	end
	else if @procname = 'sp_datatype_info'
	begin
		create table #spdatatypeinfo (
			TYPE_NAME			varchar(32)  not null,
			DATA_TYPE			smallint not null,
			"PRECISION"			int null,
			LITERAL_PREFIX		varchar(32)    null,
			LITERAL_SUFFIX		varchar(32)    null,
			CREATE_PARAMS		varchar(32)    null,
			NULLABLE			smallint   not null,
			CASE_SENSITIVE		smallint   not null,
			SEARCHABLE			smallint   not null,
			UNSIGNED_ATTRIBUTE	smallint   null,
			MONEY	smallint	not null,
			AUTO_INCREMENT		smallint	null,
			LOCAL_TYPE_NAME 	varchar(128) null,
			MINIMUM_SCALE		smallint	 null,
			MAXIMUM_SCALE		smallint   null,
			SQL_DATA_TYPE		smallint	  not null,
			SQL_DATETIME_SUB	smallint   null,
			NUM_PREC_RADIX		int	 null,
			INTERVAL_PRECISION	smallint	NULL,
			USERTYPE			tinyint not null)
		insert into #spdatatypeinfo exec sp_datatype_info @p7,@ODBCVer
		exec @ret = sp_cursoropen @handle output,
			'select * from #spdatatypeinfo',
			@scrollopt output, @ccopt output, @rows output
		drop table #spdatatypeinfo
	end
	else if @procname = 'sp_fkeys'
	begin
		create table #spfkeys (
			PKTABLE_QUALIFIER varchar(32)	 null,
			PKTABLE_OWNER varchar(32)	null,
			PKTABLE_NAME varchar(32)  not null,
			PKCOLUMN_NAME varchar(32)  not null,
			FKTABLE_QUALIFIER varchar(32)	null,
			FKTABLE_OWNER varchar(32)	null,
			FKTABLE_NAME varchar(32)  not null,
			FKCOLUMN_NAME varchar(32)  not null,
			KEY_SEQ smallint not null,
			UPDATE_RULE smallint null,
			DELETE_RULE smallint null,
			FK_NAME varchar(32) null,
			PK_NAME varchar(32) null
			)
		insert into #spfkeys exec sp_fkeys @p1,@p2,@p3,@p4,@p5,@p6
		exec @ret = sp_cursoropen @handle output,
			'select * from #spfkeys',
			@scrollopt output, @ccopt output, @rows output
		drop table #spfkeys
	end
	else if @procname = 'sp_pkeys'
	begin
		create table #sppkeys (
			TABLE_QUALIFIER varchar(32)   null,
			TABLE_OWNER varchar(32)   null,
			TABLE_NAME varchar(32)	not null,
			COLUMN_NAME varchar(32)  not null,
			KEY_SEQ smallint not null,
			PK_NAME varchar(32) null
			)
		insert into #sppkeys exec sp_pkeys @p1,@p2,@p3
		exec @ret = sp_cursoropen @handle output,
			'select * from #sppkeys',
			@scrollopt output, @ccopt output, @rows output
		drop table #sppkeys
	end
	else if @procname = 'sp_special_columns'
	begin
		create table #spspeccol (
			SCOPE smallint null,
			COLUMN_NAME varchar(32) not null,
			DATA_TYPE smallint not null,
			TYPE_NAME varchar(30) not null,
			"PRECISION" int null,
			LENGTH int null,
			SCALE smallint null,
			PSEUDO_COLUMN smallint null
			)
		insert into #spspeccol exec sp_special_columns @p1,@p2,@p3,@p4,@p5,@p6,@ODBCVer
		exec @ret = sp_cursoropen @handle output,
			'select * from #spspeccol',
			@scrollopt output, @ccopt output, @rows output
		drop table #spspeccol
	end
	else if @procname = 'sp_sproc_columns'
	begin
		create table #spproccol (
			PROCEDURE_QUALIFIER varchar(32)  null,
			PROCEDURE_OWNER varchar(32)  null,
			PROCEDURE_NAME varchar(32) not null,
			COLUMN_NAME varchar(32) not null,
			COLUMN_TYPE smallint not null,
			DATA_TYPE smallint not null,
			TYPE_NAME varchar(30) not null,
			"PRECISION" int null,
			LENGTH int null,
			SCALE smallint null,
			RADIX smallint null,
			NULLABLE smallint not null,
			REMARKS varchar(254) null,
			COLUMN_DEF varchar(254) null,
			SQL_DATA_TYPE smallint not null,
			SQL_DATETIME_SUB smallint null,
			CHAR_OCTET_LENGTH int null,
			ORDINAL_POSITION int not null,
			IS_NULLABLE varchar(254) null,
			SS_DATA_TYPE tinyint null
			)
		insert into #spproccol exec sp_sproc_columns @p1,@p2,@p3,@p4,@ODBCVer
		exec @ret = sp_cursoropen @handle output,
			'select * from #spproccol',
			@scrollopt output, @ccopt output, @rows output
		drop table #spproccol
	end
	else if @procname = 'sp_statistics'
	begin
		create table #spstatistics (
			TABLE_QUALIFIER varchar(32)   null,
			TABLE_OWNER varchar(32)   null,
			TABLE_NAME varchar(32)	not null,
			NON_UNIQUE smallint null,
			INDEX_QUALIFIER varchar(32) null,
			INDEX_NAME varchar(32) null,
			TYPE smallint not null,
			SEQ_IN_INDEX smallint null,
			COLUMN_NAME varchar(32) null,
			COLLATION char(1) null,
			CARDINALITY int null,
			PAGES int null,
			FILTER_CONDITION varchar(128) null
			)
		insert into #spstatistics exec sp_statistics @p1,@p2,@p3,@p4,@p5,@p6
		exec @ret = sp_cursoropen @handle output,
			'select * from #spstatistics',
			@scrollopt output, @ccopt output, @rows output
		drop table #spstatistics
	end
	else if @procname = 'sp_stored_procedures'
	begin
		create table #spprocedures (
			PROCEDURE_QUALIFIER varchar(32)  null,
			PROCEDURE_OWNER varchar(32)  null,
			PROCEDURE_NAME varchar(32) not null,
			NUM_INPUT_PARAMS int null,
			NUM_OUTPUT_PARAMS int null,
			NUM_RESULT_SETS int null,
			REMARKS varchar(254) null,
			PROCEDURE_TYPE smallint null
			)
		insert into #spprocedures exec sp_stored_procedures @p1,@p2,@p3
		exec @ret = sp_cursoropen @handle output,
			'select * from #spprocedures',
			@scrollopt output, @ccopt output, @rows output
		drop table #spprocedures
	end
	else if @procname = 'sp_table_privileges'
	begin
		create table #sptabpriv (
			TABLE_QUALIFIER varchar(32) null,
			TABLE_OWNER varchar(32) null,
			TABLE_NAME varchar(32) not null,
			GRANTOR varchar(32) null,
			GRANTEE varchar(32) not null,
			PRIVILEGE varchar(32) not null,
			IS_GRANTABLE varchar(3) null
			)
		insert into #sptabpriv exec sp_table_privileges @p1,@p2,@p3
		exec @ret = sp_cursoropen @handle output,
			'select * from #sptabpriv',
			@scrollopt output, @ccopt output, @rows output
		drop table #sptabpriv
	end
	else if @procname = 'sp_tables'
	begin
		create table #sptables (
			TABLE_QUALIFIER varchar(32) null,
			TABLE_OWNER varchar(32) null,
			TABLE_NAME varchar(32) null,
			TABLE_TYPE	varchar(32) null,
			REMARKS varchar(254) null)
		insert into #sptables exec sp_tables @p1,@p2,@p3,@p4
		exec @ret = sp_cursoropen @handle output,
			'select * from #sptables',
			@scrollopt output, @ccopt output, @rows output
		drop table #sptables
	end
	select @ret = isnull(@ret,0)
	return isnull(@ret,0)
go

grant execute on sp_ddopen to public
go

dump tran master with no_log
go

if exists (select * from sysobjects where name = 'sp_configure'
			and sysstat & 0xf = 4)
	begin
		exec sp_configure 'allow updates',0
		reconfigure with override
	end
go

exec sp_MS_upd_sysobj_category 2 --set category | 2 based on crdate.

go

if exists (select * from sysobjects where name = 'sp_check_objects'
			and sysstat & 0xf = 4)
	begin
		/* Only supported on 6.0 servers */
		print ''
		print 'Checking objects created by instcat.sql.'

		exec sp_check_objects 'catalog'
	end
go

print ''
print 'instcat.sql completed successfully.'
go

set quoted_identifier off
go

dump tran master with no_log
go
checkpoint
go
/**/
