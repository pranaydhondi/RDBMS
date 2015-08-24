#include "LookupIter.h"

LookupIter::LookupIter(FileHandler * _fHandler,char *_key, KeyType _keyType, TreeNode *_node, int _position, int _payloadLen) {
    key = _key;
    keyType = _keyType;
    node = _node;
    position = _position;
    payloadlen = _payloadLen;
    nullIter = false;
    fHandler = _fHandler;
}

LookupIter::LookupIter() {
    nullIter = true;
}
 int LookupIter::loadNode(TreeNode *here, char *offset) {
		int position = 0;
        char *block = (char *) malloc(BLOCK_SIZE);

        fHandler->readBlock(Utils::getIntForBytes(offset), block);
        Utils::copyBytes(here->myaddr,offset, NODE_OFFSET_SIZE);
        position += NODE_OFFSET_SIZE;

        here->flag = block[position];
        position += 1;
        here->numkeys = Utils::getIntForBytes(&(block[position]));
        position += sizeof(here->numkeys);
        Utils::copyBytes(here->data, &(block[position]), sizeof(here->data));
		position += sizeof(here->data);
		//printf("%d %d\n",Utils::getIntForBytes(&block[position]),position);
		if(Utils::getIntForBytes(&block[position]) != -1){
			here->nextaddr = (char *) malloc(NODE_OFFSET_SIZE);
			Utils::copyBytes(here->nextaddr, &(block[position]), NODE_OFFSET_SIZE);
		}
        free(block);
        return 0;
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
		 TreeNode *tempNode ;
		 tempNode = new TreeNode();
		 if(!node->nextaddr)return false;
		 loadNode(tempNode,node->nextaddr);
		 tempNode->display(keyType);
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
		 TreeNode *tempNode;
		 tempNode = new TreeNode();
		 loadNode(tempNode,node->nextaddr);
		 node = tempNode;
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
