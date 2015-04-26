#ifndef __VANILLA_UTILS_HASHTABLE_H__
#define __VANILLA_UTILS_HASHTABLE_H__
// Vanilla HashTable

typedef struct VHashTableItem
{
	VHashTableItem(){
		//this->Key = NULL;
		this->Value = NULL;
	}
	VHashTableItem(VanillaString Key, VanillaBinary Value){
		this->Key = Key;
		this->Value = Value;
	}
	VanillaString Key;
	VanillaBinary Value;
} *VanillaHashTableItem, _VHashTableItem;


typedef std::map<VanillaString, VanillaBinary> VHashTable;
typedef std::map<VanillaString, VanillaBinary>* VanillaHashTable;

typedef struct VHashTableTraveler
{
	VHashTableTraveler(){};
	std::map<VanillaString, VanillaBinary>::const_iterator x;
} *VanillaHashTableTraveler, _VHashTableTraveler;

VAPI(VanillaHashTable) VanillaHashTableCreate();
VAPI(VanillaVoid) VanillaHashTableDestroy(VanillaHashTable HashTable);
VAPI(VanillaVoid) VanillaHashTableInsert(VanillaHashTable HashTable, VanillaText Key, VanillaBinary Value);
VAPI(VanillaVoid) VanillaHashTableDelete(VanillaHashTable HashTable, VanillaText Key);
VAPI(VanillaBinary) VanillaHashTableGet(VanillaHashTable HashTable, VanillaText Key);
VAPI(VanillaVoid) VanillaHashTableSet(VanillaHashTable HashTable, VanillaText Key, VanillaBinary Value);
VAPI(VanillaInt) VanillaHashTableCount(VanillaHashTable HashTable);
VAPI(VanillaHashTableItem) VanillaHashTableTravel(VanillaHashTable HashTable, VanillaHashTableTraveler* x);
#endif	//__VANILLA_UTILS_HASHTABLE_H__
