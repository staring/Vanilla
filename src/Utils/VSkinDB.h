// Vanilla SkinDB

typedef struct VSkinDB
{
	VSkinDB();
	VSkinDB(VanillaText Name);
	~VSkinDB();

	VanillaText Name;
	VanillaHashTable HashTable;
} *VanillaSkinDB, _VSkinDB;

VanillaVoid WriteFileBin(FILE* File, VanillaBin Bin);
VanillaBin ReadFileBin(FILE* File);
VanillaVoid WriteFileText(FILE* File, VanillaText String);
VanillaText ReadFileText(FILE* File);
VanillaVoid WriteFileInt(FILE* File, VanillaInt Value);
VanillaInt ReadFileInt(FILE* File);

VAPI(VanillaSkinDB) VanillaSkinDBCreate(VanillaText Name);
VAPI(VanillaVoid) VanillaSkinDBDestroy(VanillaSkinDB SkinDB);
VAPI(VanillaVoid) VanillaSkinDBSet(VanillaSkinDB SkinDB, VanillaText Name, VanillaBin Data);
VAPI(VanillaBin) VanillaSkinDBGet(VanillaSkinDB SkinDB, VanillaText Name);
VAPI(VanillaVoid) VanillaSkinDBDelete(VanillaSkinDB SkinDB, VanillaText Name);
VAPI(VanillaBool) VanillaSkinDBSave(VanillaSkinDB SkinDB, VanillaText FileName);
VAPI(VanillaSkinDB) VanillaSkinDBLoad(VanillaText FileName);
