#include <vector>

using namespace std;

const int maxColision = 3;
const float maxFillFactor = 0.8;

template<typename TK, typename TV>
struct ChainHashNode {
   TK key; TV value; size_t hashcode;
	ChainHashNode* next;
	ChainHashNode() = default;
	ChainHashNode(TK key, TV value, size_t hashcode): key(key), value(value), hashcode(hashcode), next(nullptr) {}
	ChainHashNode(TK key, TV value, size_t hashcode, ChainHashNode* next): key(key), value(value), hashcode(hashcode), next(next) {}
	~ChainHashNode() = default;
};

template<typename TK, typename TV>
class ChainHashListIterator {
 	typedef ChainHashNode<TK, TV> Node;
	Node* current;
public:
	ChainHashListIterator(Node* current): current(current) {}
	ChainHashListIterator& operator++() {current = current->next; return *this;}
	bool operator!=(const ChainHashListIterator& other) {return current != other.current;}
	bool operator==(const ChainHashListIterator& other) {return current == other.current;}
	Node& operator*(){return *current;}

};

template<typename TK, typename TV>
class ChainHash
{
private:    
    typedef ChainHashNode<TK, TV> Node;
    typedef ChainHashListIterator<TK, TV> Iterator;

	Node** array;  // array de punteros a Node
    int nsize; // total de elementos <key:value> insertados
	int capacity; // tamanio del array
	int *bucket_sizes; // guarda la cantidad de elementos en cada bucket
	int usedBuckets; // cantidad de buckets ocupados (con al menos un elemento)

public:
    ChainHash(int initialCapacity = 10){
		this->capacity = initialCapacity; 
		this->array = new Node*[capacity]();  
		this->bucket_sizes = new int[capacity]();
		this->nsize = 0;
		this->usedBuckets = 0;
	}

	TV get(TK key){
		size_t hashcode = getHashCode(key);
		size_t index = hashcode % capacity;
		
		Node* current = this->array[index];
		while(current != nullptr){
			if(current->key == key) return current->value;
			current = current->next;
		}
		throw std::out_of_range("Key no encontrado");
	}
	
	int size(){ return this->nsize; }	

	int bucket_count(){ return this->capacity; }

	int bucket_size(int index) { 
		if(index < 0 || index >= this->capacity) throw std::out_of_range("Indice de bucket invalido");
		return this->bucket_sizes[index]; 
	}	
	
	// TODO: implementar los siguientes métodos
	void set(TK key, TV value) {
    	if (fillFactor() > maxFillFactor) {std::cout << "fill factor rehash\n"; rehashing(); }
    	size_t hashcode = getHashCode(key);
    	size_t index = hashcode % capacity;

    	//si la key ya existe
    	Node* current = this->array[index];
    	while(current != nullptr){
    		if(current->key == key) {current->value = value; return;}
    		current = current->next;
    	}

    	//si la key no existe (si existe ya habría retornado)
    	Node* newNode = new Node(key, value, hashcode);
    	newNode->next = this->array[index];
    	this->array[index] = newNode;
    	this->nsize++;
    	this->bucket_sizes[index]++;
    	//std::cout << "SET index " << index << ": added element key " << array[index]->key << " value " << array[index]->value << "\n";
    	//std::cout << "SET index " << index << ": new bucket size: " << bucket_sizes[index] << "\n";
    	if (this->array[index]->next == nullptr) this->usedBuckets++;
    	if (this->bucket_sizes[index] > maxColision) {
    		//std::cout << "bucket size " << index << " on set rehash\n";
    		rehashing();
    		//std::cout << "this line actually prints\n";
    	}
    }

	bool remove(TK key) {
	    size_t hashcode = getHashCode(key);
    	size_t index = hashcode % capacity;
    	Node* current = this->array[index];
    	if (current != nullptr) return false;
    	if (current->key == key) {
    		auto temp = array[index];
    		array[index] = array[index]->next;
    		delete temp; temp = nullptr; return true;
    	}
    	while(current->next != nullptr){
    		if (current->next->key == key) {
    			auto temp = current->next;
    			current->next = current->next->next;
    			delete temp; temp = nullptr; return true;
    		}
    		current = current->next;
    	} return false;
    }
	bool contains(TK key) {
    	size_t hashcode = getHashCode(key);
    	size_t index = hashcode % capacity;
    	Node* current = this->array[index];
    	while(current != nullptr){
    		if(current->key == key) return true;
    		current = current->next;
    	} return false;
    }
	Iterator begin(int index) {return Iterator(array[index]);}
	Iterator end(int index) {return Iterator(nullptr);}

private:
	double fillFactor(){
		return (double)this->usedBuckets / (double)this->capacity;
	}	

	size_t getHashCode(TK key){
		std::hash<TK> ptr_hash;
		return ptr_hash(key);
	}

	//TODO: implementar rehashing
	void rehashing() {
		/*std::cout << "REHASH\n";
		std::cout << "previous capacity: " << capacity << " new capacity: " << capacity*2 << std::endl;
		std::cout << "\narray before rehash:\n\n";
		for (int i=0; i<capacity; ++i) {
			auto curr = array[i];
			std::cout << "index " << i << ": ";
			while (curr != nullptr) {std::cout << "key " << array[i]->key << " val " << array[i]->value << ","; curr=curr->next;}
			std::cout << "\n";
		}
		std::cout << "\n";*/
		auto old_capacity = capacity;
		capacity*=2;
		Node** new_array = new Node*[capacity]();
		int* new_bucket_sizes = new int[capacity]();
		for (int i=0; i<capacity; ++i) new_bucket_sizes[i] = 0;
		int new_used_buckets = 0;
		for (int i=0; i<old_capacity; ++i) {
			Node* current = array[i];
			while (current != nullptr) {
				Node* next = current->next;
				int new_index = current->hashcode % capacity;
				current->next = new_array[new_index];
				new_array[new_index] = current;
				if (current->next == nullptr) ++new_used_buckets;
				++new_bucket_sizes[new_index];
				//std::cout << "index " << new_index << ": added element key " << new_array[new_index]->key << " value " << new_array[new_index]->value << "\n";
				//std::cout << "index " << new_index << ": new bucket size: " << new_bucket_sizes[new_index] << "\n";
				current = next;
			}

		}

		bucket_sizes = new_bucket_sizes;
		array = new_array;
		usedBuckets = new_used_buckets;

		/*std::cout << "\narray after rehash:\n\n";
		for (int i=0; i<capacity; ++i) {
			auto curr = array[i];
			std::cout << "index " << i << ": ";
			while (curr != nullptr) {std::cout << "key " << array[i]->key << " val " << array[i]->value << ","; curr=curr->next;}
			std::cout << "\n";
		}
		std::cout << "-----------------------------------\n";*/

		for (int i=0; i<capacity; ++i) {
			if (bucket_sizes[i] > maxColision) {
				//std::cout << "\nrecursive rehash bucket " << i << " \n\n";
				rehashing();
				return;
			}
		}
	}

public:
	// TODO: implementar destructor
	~ChainHash() {
		for (int i=0; i<capacity; ++i) {
			while (array[i] != nullptr) {
				auto temp = array[i];
				array[i] = array[i]->next;
				delete temp;
				//array[i] = nullptr;
			}
		}
		delete[] array;
		array = nullptr;
		delete[] bucket_sizes;
		bucket_sizes = nullptr;
	}
};
