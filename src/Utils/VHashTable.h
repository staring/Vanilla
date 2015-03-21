#ifndef __VANILLA_UTILS_HASHTABLE_H__
#define __VANILLA_UTILS_HASHTABLE_H__
// Vanilla HashTable

typedef struct VHashTableItem
{
	VHashTableItem(){
		//this->Key = NULL;
		this->Value = NULL;
	}
	VHashTableItem(VanillaString Key, VanillaBin Value){
		this->Key = Key;
		this->Value = Value;
	}
	VanillaString Key;
	VanillaBin Value;
} *VanillaHashTableItem, _VHashTableItem;


typedef std::map<VanillaString, VanillaBin> VHashTable;
typedef std::map<VanillaString, VanillaBin>* VanillaHashTable;

typedef struct VHashTableTraveler
{
	VHashTableTraveler(){};
	std::map<VanillaString, VanillaBin>::const_iterator x;
} *VanillaHashTableTraveler, _VHashTableTraveler;

VAPI(VanillaHashTable) VanillaHashTableCreate();
VAPI(VanillaVoid) VanillaHashTableDestroy(VanillaHashTable HashTable);
VAPI(VanillaVoid) VanillaHashTableInsert(VanillaHashTable HashTable, VanillaText Key, VanillaBin Value);
VAPI(VanillaVoid) VanillaHashTableDelete(VanillaHashTable HashTable, VanillaText Key);
VAPI(VanillaBin) VanillaHashTableGet(VanillaHashTable HashTable, VanillaText Key);
VAPI(VanillaVoid) VanillaHashTableSet(VanillaHashTable HashTable, VanillaText Key, VanillaBin Value);
VAPI(VanillaInt) VanillaHashTableCount(VanillaHashTable HashTable);
VAPI(VanillaHashTableItem) VanillaHashTableTravel(VanillaHashTable HashTable, VanillaHashTableTraveler* x);
#endif	//__VANILLA_UTILS_HASHTABLE_H__
