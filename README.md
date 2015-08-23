# CS631-2015
Assignments for CS631 at IIT Bombay

Team members : 

Pranay Dhondi 120050054
KV Gangadhar 120050078

Implementation:

-> Insert
	
	1) checked whether the insert poistion of the incoming element is 0 and whether the incoming element matches with the last key of the node then we know that node contains only key value corresponding to the incoming node then we simply shown message that you can't insert.
	2) Other wise we inserted key into the node to form node of (n+1) then we split the node into half and checked whether the values of keys on the either side of the slipt node are equal. If they are equal then we put the key values having the same value in one node and other is other bucket
	3) now used the split function which splits the tree at desired postion and also manages the nodes at differnt levels accordingly

-> Find
	1) for hasnext we checked whether the current iterator is null , if yes we returned false ,if no then we check whether next value is same the current key then returned yes otherwise no. Corner conditions are also taken into account.
	2) for get we checked whether iterator is null , if yes then return value is set to -1 else returned the value of payload
	3) for next we checked whether it has next , if yes then we increased postion by 1 else returned nullpointer
	
	* for Next using the findfirstelement function that we have implemented modified to give the iterator

