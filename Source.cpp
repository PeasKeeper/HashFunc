#include <iostream> #include <cstdio> #include <stdexcept>
using namespace std; struct Element {
	char key[7] = {};
	int value;
	int collisions; bool deleted;
};

struct ListElement {
	ListElement* forward; Element* value;
};

class BadList {
private:
	ListElement* head; ListElement* tail;
public:
	BadList();
	~BadList();
	void push(Element*);
	ListElement* getNext(ListElement*); Element* getValue(ListElement*); ListElement* getHead();
	void removeElement(ListElement*);
};

void BadList::removeElement(ListElement* elementForDeleting) {
	ListElement* currentElement = head;
	ListElement* previousElement; if (currentElement != NULL) {
		while (true) {
			if (currentElement == elementForDeleting) {
				if (currentElement == head) {
					head = getNext(head); delete elementForDeleting; return;
				}
				if (currentElement == tail) {
					tail = previousElement; tail->forward = NULL; delete elementForDeleting; return;

				}
				previousElement->forward = getNext(currentElement); delete elementForDeleting;
			}
			previousElement = currentElement; currentElement = getNext(currentElement); if (currentElement == NULL) {
				return;
			}
		}
	}
}

BadList::~BadList() {
	ListElement* currentElement = head; ListElement* nextElement;
	if (head != NULL) {
		while (true) {
			if (getNext(currentElement) == NULL) {
				delete currentElement;
				break;
			}
			else {
				nextElement = getNext(currentElement); delete currentElement;
				currentElement = nextElement;
			}
		}
	}
}

BadList::BadList() {
	head = NULL; tail = NULL;
}

ListElement* BadList::getNext(ListElement* element) {
	return element->forward;
}

Element* BadList::getValue(ListElement* element) {
	return element->value;
}

ListElement* BadList::getHead() {
	return head;
}

void BadList::push(Element* entry) {
	if (head == NULL) {
		head = new ListElement;

		head->value = entry; tail = head;
		tail->forward = NULL;
	}
	else {
		ListElement* nextElement = new ListElement; tail->forward = nextElement;
		tail = nextElement; tail->value = entry; tail->forward = NULL;
	}
}

class Hash {
private:
	int arrSize; BadList badList;
	Element* allElements; Element* tempElements;
	int quadraticHashFunction(char*); int cubicHashFunction(char*); bool checkKey(char*);
	bool isLetter(char); bool isNumber(char);
bool checkKeyDublicate(char*); public:
	Hash(int);
	~Hash();
	bool getElementAtKey(char*, int&); void deleteElement(char*);
	void deleteElementOptimized(char*); void printHash();
	void add(int, char*); void exportToCSV();
};

void Hash::deleteElementOptimized(char* key) {
	deleteElement(key);
	BadList listForOptimization;
	int hashForDeletion = quadraticHashFunction(key); for (int i = 0; i < arrSize; i++) {
		if (hashForDeletion == quadraticHashFunction(allElements[i].key)) {
			Element* newOptimizationElement = new Element; strcpy(newOptimizationElement->key, allElements[i].key); newOptimizationElement->value = allElements[i].value; listForOptimization.push(newOptimizationElement);
		}
	}
	ListElement* currentElement = badList.getHead(); if (currentElement != NULL) {
		do {
			if (quadraticHashFunction(badList.getValue(currentElement)->key) == hashForDeletion) {
				Element* newOptimizationElement = new Element; strcpy(newOptimizationElement->key,
					badList.getValue(currentElement)->key);
				newOptimizationElement->value = badList.getValue(currentElement) -> value;
				listForOptimization.push(newOptimizationElement);
			}

		} while (currentElement = badList.getNext(currentElement));
	}
	currentElement = listForOptimization.getHead(); if (currentElement != NULL) {
		do {
			deleteElement(listForOptimization.getValue(currentElement)->key);
		} while (currentElement = listForOptimization.getNext(currentElement));
	}
	currentElement = listForOptimization.getHead(); if (currentElement != NULL) {
		do {
			add(listForOptimization.getValue(currentElement)->value, listForOptimization.getValue(currentElement)->key);
			delete listForOptimization.getValue(currentElement);
		} while (currentElement = listForOptimization.getNext(currentElement));
	}
}

void Hash::deleteElement(char* key) {
	int hashCode;
	hashCode = quadraticHashFunction(key);
	if (strcmp(allElements[hashCode].key, "") == 0) {
		if (!allElements[hashCode].deleted) {
			return;
		}
	}
	if (strcmp(allElements[hashCode].key, key) == 0) {
		allElements[hashCode].key[0] = '\0'; allElements[hashCode].value = 0;
		allElements[hashCode].collisions = 0; allElements[hashCode].deleted = true; return;
	}
	int cubicKey = cubicHashFunction(key); for (int i = 0; i <= arrSize; i++) {
		hashCode += cubicKey; hashCode = hashCode % arrSize;
		if (strcmp(allElements[hashCode].key, "") == 0) {
			if (!allElements[hashCode].deleted) {
				return;
			}
		}

		if (strcmp(allElements[hashCode].key, key) == 0) {
			allElements[hashCode].key[0] = '\0'; allElements[hashCode].value = 0;
			allElements[hashCode].collisions = 0; allElements[hashCode].deleted = true; return;
		}
	}
	ListElement* currentElement = badList.getHead(); if (currentElement != NULL) {
		do {
			if (strcmp(badList.getValue(currentElement)->key, key) == 0) {
				delete badList.getValue(currentElement); badList.removeElement(currentElement);
				return;
			}
		} while (currentElement = badList.getNext(currentElement));
	}
}

void Hash::exportToCSV() {
	FILE* File;
	File = fopen("AIST.csv", "w"); for (int i = 0; i < arrSize; i++) {
		fprintf(File, "%d", allElements[i].collisions); fputs("\n", File);
	}
	fclose(File);
}

bool Hash::isLetter(char letter) {
	if (letter >= 65 && letter <= 90) {
		return true;
	}
	return false;
}

bool Hash::isNumber(char number) {
	if (number >= 48 && number <= 57) {
		return true;
	}
	return false;
}

bool Hash::checkKey(char* key) {
	if (!isNumber(key[0]) || !isNumber(key[1]) || !isLetter(key[2]) ||
		!isLetter(key[3]) || !isNumber(key[4]) || !isNumber(key[5])) {
		return false;
	}
	return true;
}

void Hash::printHash() {
	for (int i = 0; i < arrSize; i++) {
		if (strcmp(allElements[i].key, "") != 0) {
			cout << "( " << allElements[i].key << ", " << allElements[i].value
				<< " )";
		}
	}
	cout << "\n";
	ListElement* currentElement = badList.getHead(); if (currentElement != NULL) {
		do {
			cout << "( " << badList.getValue(currentElement)->key << ", " << badList.getValue(currentElement)->value << " )";
		} while (currentElement = badList.getNext(currentElement));
	}
	cout << "\n";
}

bool Hash::getElementAtKey(char* key, int& result) {
	int hashCode;
	hashCode = quadraticHashFunction(key);
	if (strcmp(allElements[hashCode].key, "") == 0) {
		if (!allElements[hashCode].deleted) {
			return false;
		}
	}
	if (strcmp(allElements[hashCode].key, key) == 0) {
		result = allElements[hashCode].value;
		return true;
	}
	int cubicKey = cubicHashFunction(key); for (int i = 0; i <= arrSize; i++) {
		hashCode += cubicKey; hashCode = hashCode % arrSize;
		if (strcmp(allElements[hashCode].key, "") == 0) {
			if (!allElements[hashCode].deleted) {
				return false;
			}
		}
		if (strcmp(allElements[hashCode].key, key) == 0) {
			result = allElements[hashCode].value;
			return true;
		}
	}
	ListElement* currentElement = badList.getHead(); if (currentElement != NULL) {
		do {
			if (strcmp(badList.getValue(currentElement)->key, key) == 0) {
				result = badList.getValue(currentElement)->value;
				return true;

			}
		} while (currentElement = badList.getNext(currentElement));
	}
	return false;
}

void Hash::add(int value, char* key) {
	int hashCode;
	if (checkKey(key)) {
		int d;
		if (getElementAtKey(key, d)) {
			throw invalid_argument("Такой ключ уже есть \n");
		}
		hashCode = quadraticHashFunction(key);
		if (strcmp(allElements[hashCode].key, "") == 0) {
			strcpy(allElements[hashCode].key, key); allElements[hashCode].value = value; allElements[hashCode].deleted = false;
		}
		else {
			bool flag = true;
			int cubicKey = cubicHashFunction(key); allElements[hashCode].collisions++; for (int i = 0; i < arrSize; i++) {
				hashCode += cubicKey; hashCode = hashCode % arrSize;
				if (strcmp(allElements[hashCode].key, "") == 0) {
					strcpy(allElements[hashCode].key, key); allElements[hashCode].value = value; allElements[hashCode].deleted = false;
					flag = false; break;
				}
			}
			if (flag) {
				Element* newBadElement = new Element; strcpy(newBadElement->key, key); newBadElement->value = value; newBadElement->deleted = false; badList.push(newBadElement);
			}
		}
	}
}

Hash::Hash(int size) : badList() {
	arrSize = size;
	allElements = new Element[arrSize]; tempElements = new Element[arrSize]; for (int i = 0; i < arrSize; i++) {
		allElements[i].key[0] = '\0';

		allElements[i].value = 0;
		allElements[i].collisions = 0; allElements[i].deleted = false;
	}
}

Hash::~Hash() {
	delete[] allElements; delete[] tempElements;
	ListElement* currentElement = badList.getHead(); ListElement* nextElement;
	if (currentElement != NULL) {
		while (true) {
			if (badList.getNext(currentElement) == NULL) {
				delete badList.getValue(currentElement);; break;
			}
			else {
				delete badList.getValue(currentElement); currentElement = badList.getNext(currentElement);
			}
		}
	}
}

int Hash::quadraticHashFunction(char* Key) {
	int hash, i;
	i = 0;
	hash = 0;
	while (Key[i] != 0) {
		hash += Key[i] * Key[i]; i++;
	}
	hash = hash % arrSize; return hash;
}

int Hash::cubicHashFunction(char* Key) {
	int hash, i;
	i = 0;
	hash = 0;
	while (Key[i] != 0) {
		hash += Key[i] * Key[i] * Key[i]; i++;
	}
	hash = hash % arrSize; return hash;
}

void generateKey(char* key) {
	char currentSymbol;

	currentSymbol = 48 + rand() % 10; key[0] = currentSymbol; currentSymbol = 48 + rand() % 10; key[1] = currentSymbol; currentSymbol = 65 + rand() % 26; key[2] = currentSymbol; currentSymbol = 65 + rand() % 26; key[3] = currentSymbol; currentSymbol = 48 + rand() % 10; key[4] = currentSymbol; currentSymbol = 48 + rand() % 10; key[5] = currentSymbol;
	key[6] = 0;
}

int main() {
	int hashSize, numberOfKeys, choice, value, elementFlag, mValue, delChoice; cout << "Введите количество элементов в хэше: \n";
	cin >> hashSize; Hash hash(hashSize);
	cout << "Введите количество ключей: \n"; cin >> numberOfKeys;
	char key[7] = {};
	char mKey[7] = {};
	char dKey[7] = {}; srand((unsigned)time(NULL));
	for (int i = 0; i < numberOfKeys; i++) {
		generateKey(key);
		hash.add(10 + rand() % 10, key);
	}
	while (true) {
		cout << "Выберите действие (введите номер): \n"; cout << "1. Вывести весь хэш \n";
		cout << "2. Вывести элемент по ключу \n"; cout << "3. Удалить элемент по ключу \n"; cout << "4. Добавить ключ \n";
		cout << "5. Экспортировать данные хэша в CSV файл \n"; cout << "6. Закончить работу \n";
		cin >> choice; switch (choice) {
		case 1:
			hash.printHash(); cout << "\n"; break;
		case 2:
			cout << "Введите ключ: \n"; cin >> key;
			elementFlag = hash.getElementAtKey(key, value); if (elementFlag) {
				cout << "Полученное значение: \n"; cout << value;

				cout << "\n";
			}
			else {
				cout << "Нет элемента с таким ключом \n";
			}
		break; case 3:
			cout << "Ведите ключ удаляемого элемента"; cin >> dKey;
			cout << "Выберите действие (введите номер): \n"; cout << "1. Удалить без оптимизации \n";
			cout << "2. Удалить с оптимизацией \n"; cin >> delChoice;
			switch (delChoice) {
			case 1:
				hash.deleteElement(dKey);
				cout << "Новое содержание хэша: \n"; hash.printHash();
				cout << "\n"; break;
			case 2:
				hash.deleteElementOptimized(dKey); cout << "Новое содержание хэша: \n"; hash.printHash();
				cout << "\n";
			}
		break; case 4:
			cout << "Введите ключ нового элемента"; cin >> mKey;
			cout << "Введите значение нового элемента"; cin >> mValue;
			hash.add(mValue, mKey);
			cout << "Новое содержание хэша: \n"; hash.printHash();
			cout << "\n"; break;
		case 5:
			hash.exportToCSV(); break;
		case 6:
			return 0;
		}
	}
	return 0;
}