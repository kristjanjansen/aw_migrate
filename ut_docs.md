# Kasutajanimed 

tabeli nimi: ut_ldap_uid_to_aw_uid

nii kasutajate kui failide / contenti migreerimise puhul
tuleb asendada aw usernamed ldapi usernamedega. kui
ldapi username ei ole siis vist skip (username) või default 
author contenti puhul (uid 1).


# Alamsaidid

tabeli nimi: aw_site_list

Saidid ja nende id-d, mille content vaja I etapis üle tuua:

www.ut.ee - 900

www.us.ut.ee - 144

www.oi.ut.ee - 434

www-med.ut.ee - 190

www.fl.ut.ee - 147

www.kk.ut.ee - 249

www.lote.ut.ee - 390

www.mtk.ut.ee - 119

www.math.ut.ee - 97

www.sh.ut.ee - 43

www.ec.ut.ee - 908

www.narva.ut.ee - 286

www.pc.ut.ee - 384

www.kultuur.edu.ee - 343


# Sisu hierarhia

ut.ee veebis saab põhilise sisupuu kätte oid 507837 alt.

Sisu on loodud peamiselt Peamenüü, ülemine menüü ja alumine menüü alla. Kuna osa saite on saanud juba uuendatud, siis neil on eraldi veel saidi nimi (uus) kaust, kus on siis uuendatud sisu.
Kindlam on Valmarilt küsida aw access ja ise adminnipoolt seda puud vaadata


## AW puu näitamine:

kui parentil on 2+ childi
```  
 parent (oid 1) / child1 (oid 2)
 parent (oid 1) / child2 (oid 3)
```  
siis 

/1 urli all näidatakse dokumenti kus on mõlema childi pealkiri
```  
 link to /2
 link to /1
```  
kui parentil on 1 child:
```  
 parent (oid 1) / child1 (oid 2)
```  
siis nii /1 kui /2 uril all näidatakse oid 2 contentit

Drupali puhul vist ärme teeme seda trikki, oid 1 (nid 1) oleks lihtsalt tühi dokument
ja temal oleks üks child (nid 2) mille pealkirja me näitaks menu_block.module abiga
content regionis.


# Autorid

AWs on "created by" ja "modified by". See info peaks säilitama. Kaks varianti:

a) teha migreeritavast nodest 2 versiooni, v1 uid oleks created by ja v2 uid oleks
modified by. content oleks neil sama
b) ei jama versioonidega (sest aws neid tegelt pole), teha cckväli "modified by" ja mingi
hook_node_save mis seda välja uuendab.


# Regulaane XML import

data moodul millega saab feeds'iga info sisse importida hakkab d7 jaoks looma http://drupal.org/node/827000#comment-5231950


# Veebivormid

Veebivormid, mis vaja üle tuua / uuesti luua: kokku on üle saitide 172 veebivormi, neist 37 on loodud ut.ee alla. Nendest aktiivsete, ehk siis veebi kuvatavate vormide arv on kindlasti väiksem, aga selle selgitamine võtab kauem aega.
Toon mõned näited, mis katab enamus vorme, st kui neid suudab Drupali vahenditega teha, siis teiste loomine peaks olema juba väga lihtne.

http://www.ut.ee/1062281 - vormi id on: 1062282

http://www.ut.ee/1052281 - vormi id on: 1052257

http://www.ut.ee/999017 -  vormi id on 999017

http://www.ut.ee/626906 - vormi id on 626906


# Migrated object classes
```  
classes[6][def] = CL_IMAGE
(27,850) <- sql count

classes[7][def] = CL_DOCUMENT
(60,487)

classes[41][def] = CL_FILE
(30,780)
```  
# Non-migrated object classes
```  
classes[1][def] = CL_MENU
(50,097)

classes[2][def] = CL_FORM
(244)

classes[3][def] = CL_TABLE
(276)

classes[8][def] = CL_FORM_ENTRY
(379,892)
		
classes[10][def] = CL_FORM_ELEMENT
(2,622)

classes[11][def] = CL_STYLE

classes[12][def] = CL_FORM_OUTPUT

classes[14][def] = CL_FORM_ACTION

#15 - used to be CL_MAILINGLIST - List - lihtne
(14)

classes[21][def] = CL_EXTLINK
(43,171)

classes[22][def] = CL_PROMO
(748)

classes[28][def] = CL_GRAPH

classes[29][def] = CL_PERIODIC_SECTION

classes[31][def] = CL_GALLERY

classes[33][def] = CL_POLL
(314)

classes[34][def] = CL_MSGBOARD_TOPIC

classes[37][def] = CL_GROUP
(22,137)

classes[38][def] = CL_USER_GROUP
classes[38][name] = Kasutaja1
(74)

classes[39][def] = CL_BROTHER
(10)

classes[40][def] = CL_BROTHER_DOCUMENT

51 - used to be CL_ACCESSMGR
(129)

classes[52][def] = CL_MESSAGE

#53 - used to be - CL_CAL_EVENT

classes[54][def] = CL_BANNER

classes[55][def] = CL_BANNER_CLIENT

classes[59][def] = CL_CALENDAR_VIEW

classes[66][def] = CL_FORM_TABLE

classes[68][def] = CL_FORM_CHAIN

classes[69][def] = CL_FORUM

classes[71][def] = CL_CSS

classes[72][def] = CL_ML_LIST

classes[73][def] = CL_ML_MEMBER
(44,618)

classes[78][def] = CL_SEARCH_FILTER

classes[80][def] = CL_HTML_POPUP

81 ???

classes[82][def] = CL_KEYWORD

#83 - used to be - CL_KEYWORD_DB

84 ???

classes[85][def] = CL_PULLOUT

#86 - used to be - CL_OBJECT_CHAIN

#87 - used to be - CL_ROLE

#88 - used to be - CL_ACL

#89 - used to be - CL_MENU_ALIAS - Men&uuml;&uuml; alias
(312)

classes[90][def] = CL_AW_LOGIN

classes[92][def] = CL_CHAIN_ENTRY

classes[93][def] = CL_MENU_TREE

classes[94][def] = CL_FORM_CONTROLLER

classes[95][def] = CL_HTML_IFRAME

classes[97][def] = CL_XML_IMPORT

classes[98][def] = CL_DATASOURCE

classes[103][def] = CL_EXPORT_RULE

classes[108][def] = CL_SCHEDULER

#113 - used to be CL_OBJ_TABLE_CONF

#114 - used to be - CL_PRIORITY

classes[115][def] = CL_OBJECT_TYPE

#116 - used to be CL_TREE_ROOT puu rootmenu

#117 - used to be CL_LINK_LIST - Lingikogu

classes[118][def] = CL_ADD_TREE_CONF

classes[119][def] = CL_CFGFORM

#120 - used to be - CL_CFGOBJECT

classes[126][def] = CL_PLANNER

classes[129][def] = CL_CRM_COMPANY

classes[130][def] = CL_CRM_DB

classes[131][def] = CL_CRM_SECTOR

# 132 - used to be - CL_ESITLUS

classes[134][def] = CL_CRM_COUNTRY

classes[135][def] = CL_CRM_CORPFORM

classes[139][def] = CL_CRM_CITY

classes[140][def] = CL_CRM_COUNTY

classes[145][def] = CL_CRM_PERSON
(26,883)

classes[146][def] = CL_CRM_ADDRESS

classes[153][def] = CL_DB_TABLE_CONTENTS

#157 - used to be - CL_REPEATER_OBJ

classes[162][def] = CL_DRONLINE

classes[163][def] = CL_DRONLINE_CONF

classes[164][def] = CL_DRONLINE_LOG

#165 - used to be CL_POLL_NG

classes[168][def] = CL_IPADDRESS

classes[172][def] = CL_ACTOR

classes[178][def] = CL_LAYOUT

classes[179][def] = CL_RELATION
(32,790)

classes[182][def] = CL_SITE_CONTENT

classes[187][def] = CL_GALLERY_CONF

classes[189][def] = CL_RATE

classes[190][def] = CL_GALLERY_V2

# 192 used to be - CL_CAL_CONTENT

classes[194][def] = CL_FLASH

classes[195][def] = CL_FTP_LOGIN

classes[196][def] = CL_LIVELINK_IMPORT

classes[197][def] = CL_USER
(65,375)

classes[203][def] = CL_CLASSIFICATOR

classes[205][def] = CL_SITE_SEARCH

classes[207][def] = CL_PERIOD

classes[208][def] = CL_COMMENT
(76,217)

classes[209][def] = CL_OBJECT_TREE

classes[211][def] = CL_FORUM_V2

#215 - used to be CL_SHORTCUT

classes[219][def] = CL_CRM_PHONE
(17,731)

classes[220][def] = CL_CRM_PROFESSION

classes[221][def] = CL_DOCUMENT_IMPORT

classes[224][def] = CL_CRM_MEETING

classes[227][def] = CL_MESSENGER_V2

classes[231][def] = CL_SITE_SEARCH_CONTENT

classes[232][def] = CL_SITE_SEARCH_CONTENT_GRP

classes[233][def] = CL_INTRANET

classes[239][def] = CL_PROJECT

classes[244][def] = CL_TASK

classes[251][def] = CL_CONFIG_LOGIN_MENUS

classes[252][def] = CL_CONFIG_AW_DOCUMENT_TEMPLATE

classes[255][def] = CL_FOLDER_LIST

classes[266][def] = CL_LANGUAGE

classes[275][def] = CL_PERSONNEL_MANAGEMENT

277 ???

classes[278][def] = CL_SERVER_FOLDER

classes[279][def] = CL_METAMGR

285 ???

classes[286][def] = CL_RECURRENCE

classes[287][def] = CL_JOIN_SITE

classes[288][def] = CL_JOIN_SITE_RULE

classes[295][def] = CL_SHOP_PRODUCT

classes[305][def] = CL_CONTENTS

classes[313][def] = CL_DOCUMENT_ARCHIVE

classes[318][def] = CL_MINI_GALLERY

classes[321][def] = CL_CRM_SECTION

classes[322][def] = CL_MESSAGEBOARD

classes[324][def] = CL_OBJECT_TREEVIEW_V2

classes[325][def] = CL_OTV_DS_OBJ

classes[329][def] = CL_OBJECT_IMPORT

classes[339][def] = CL_ABSTRACT_DATASOURCE

classes[341][def] = CL_REGISTER

classes[342][def] = CL_REGISTER_DATA
(18,101)

classes[343][def] = CL_REGISTER_SEARCH

classes[351][def] = CL_PERSONNEL_MANAGEMENT_JOB_WANTED

classes[477][def] = CL_OBJECT_EXPORT

classes[483][def] = CL_CRM_CATEGORY

classes[486][def] = CL_REGISTER_SEARCH_RESULT

classes[487][def] = CL_CRM_PERSON_EDUCATION

classes[489][def] = CL_CRM_PERSON_LANGUAGE

classes[645][def] = CL_CRM_PERSON_ADD_EDUCATION

classes[802][def] = CL_CALENDAR_VACANCY

classes[809][def] = CL_IMAGE_RESIZER

classes[810][def] = CL_CFGCONTROLLER

classes[812][def] = CL_PERSONNEL_MANAGEMENT_CANDIDATE

classes[819][def] = CL_CALENDAR_EVENT
(13,171)

classes[833][def] = CL_VIDEO

classes[843][def] = CL_AUTH_CONFIG

classes[844][def] = CL_AUTH_SERVER_LDAP

classes[845][def] = CL_AUTH_SERVER_LOCAL

classes[848][def] = CL_CALENDAR_REGISTRATION_FORM

classes[849][def] = CL_CALENDAR_REGISTRATION_FORM_CONF

850 ???

856 ???

classes[858][def] = CL_TIMING

classes[865][def] = CL_CFG_VIEW_CONTROLLER

classes[866][def] = CL_WEBFORM

classes[877][def] = CL_CRM_CONTRACT_STOP

classes[885][def] = CL_NEWSFEED

classes[896][def] = CL_XML_EXPORT

# used to be - 912 - CL_SIMPLE_SHOP_PRODUCT

classes[922][def] = CL_FLYER

classes[955][def] = CL_AUTH_SERVER_OPENLDAP

classes[975][def] = CL_SITE_SEARCH_CONTENT_GRP_HTML

classes[976][def] = CL_SITE_SEARCH_CONTENT_GRP_FS

classes[1003][def] = CL_USER_MANAGER

classes[1009][def] = CL_CRM_BILL

classes[1014][def] = CL_OPENHOURS

classes[1028][def] = CL_CRM_COMPANY_CUSTOMER_DATA

classes[1054][def] = CL_CRM_BILL_ROW

classes[1055][def] = CL_PERSONS_WEBVIEW

classes[1060][def] = CL_CRM_PERSON_WORK_RELATION
(31,120)

classes[1095][def] = CL_SCM_LOCATION

classes[1107][def] = CL_CRM_AREA

classes[1124][def] = CL_IMAGE_VERIFICATION

classes[1129][def] = CL_CUSTOMER_FEEDBACK_ENTRY

classes[1130][def] = CL_CUSTOMER_FEEDBACK_MANAGER

classes[1131][def] = CL_USER_BOOKMARKS

classes[1132][def] = CL_AW_OBJECT_SEARCH

classes[1133][def] = CL_OBJ_QUICK_ADD

classes[1134][def] = CL_ADMIN_IF

classes[1145][def] = CL_STATS_VIEWER

classes[1146][def] = CL_STATS_ARCH_STATUS

classes[1153][def] = CL_QUESTIONARY

classes[1167][def] = CL_CONFIG_OLD_REDIRECT

classes[1193][def] = CL_ID_CONFIG

classes[1201][def] = CL_OBJECT_BASKET

classes[1214][def] = CL_LOGIN_BOX

classes[1245][def] = CL_SITE_SEARCH_CONTENT_GRP_MULTISITE

classes[1322][def] = CL_EVENT_TIME

classes[1382][def] = CL_FLV_FILE

classes[1384][def] = CL_QUICK_RESERVATION

classes[1385][def] = CL_CRM_DEGREE
(10,255)

classes[1386][def] = CL_PERSONNEL_IMPORT

classes[1400][def] = CL_CRM_SKILL

classes[1401][def] = CL_CRM_SKILL_LEVEL

classes[1405][def] = CL_CRM_COMPANY_RELATION

classes[1406][def] = CL_CRM_RECOMMENDATION

classes[1407][def] = CL_CRM_SKILL_MANAGER

classes[1408][def] = CL_PERSONNEL_MANAGEMENT_CV_SEARCH_SAVED

1421 ???
```  