#include "LookupIter.h"

LookupIter::LookupIter(char *_key, KeyType _keyType, TreeNode *_node, int _position, int _payloadLen) {
    key = _key;
    keyType = _keyType;
    node = _node;
    position = _position;
    payloadlen = _payloadLen;
    nullIter = false;
}

LookupIter::LookupIter() {
    nullIter = true;
}

bool LookupIter::isNull() {
    return nullIter;
}

bool LookupIter::hasNext() {
    /*
     * TODO: Fill in code
     *
     * Hint: See usage in Index.cpp.main() to understand functionality
     * Also see LookupIter.h
     */
     if(isNull()) return false;
     int size = node->numkeys;
     if(position<=size-2){
		 char tempNodekey[keylen(&keyType)];
		 node->getKey(keyType,tempNodekey,position+1);
		 if(compare(key,tempNodekey,keyType)==0){
			 return true;
		 }
		 else return false;
	 }else if(position == size -1){
		 printf("endl\n");
		 TreeNode *tempNode;
		 if(node->next == NULL)return false;
		 tempNode = node->next;
		 char tempNodekey[keylen(&keyType)];
		 tempNode->getKey(keyType,tempNodekey,0);
		 if(compare(key,tempNodekey,keyType)==0){
			 return true;
		 }
		 else return false;
	 }
	 else{
		 
		 return false;
		 }
}

int LookupIter::next() {
    /*
     * TODO: Fill in code
     *
     * Hint: See usage in Index.cpp.main() to understand functionality
     * Also see LookupIter.h
     */
     int size = node->numkeys;
     if(!hasNext()) {
		 nullIter = true;
		 return -1;
	 }else if(position == size -1){
		 node = node->next;
		 position = 0;
	 }
     else{
		 position++;
		 return 0;
		 }
}


int LookupIter::get(char *payload) {
    /*
     * TODO: Fill in code
     */
     if(isNull()){
		 return -1;
		 }
	 Utils::copyBytes(payload,&(node->data[ DATA_SIZE - ((position+1) *payloadlen)]),payloadlen);
	 return 0;
}
