/**
 * @file list.hpp
 * @author Martina Lisa Saffo Ramponi (0001220008)
 * @brief defines a simple generic list 
 * container implemented as a template
 * @version 1.0
 * @date 2026-01-28
 * 
 * @copyright Copyright (c) 2026
 * 
 * 
 * this file contains the implementation of a basic singly linked list used
 * throughout the project to store game objects such as enemies, bombs, and
 * power-ups
 * 
 * the container is fully defined in the header file
 * 
 */

#ifndef LIST_HPP

#define LIST_HPP

#include <exception>

template <typename T>
class list {
    private:
        struct _node {
            T payload;
            _node* next;
            _node() : next(nullptr) { }

            _node(T payload, _node* next = nullptr){
                this->payload = payload;
                this->next = next;
            }
        };

        list<T>::_node* head;
        unsigned int size;

    public:
        class EmptyListException : public std::exception {
            public:
                const char* what() const noexcept override {
                    return "list is empty";
                }
        };

        class OutOfRangeException : public std::exception {
            public:
                const char* what() const noexcept override {
                    return "index out of range";
                }
        };

        class iterator {
            private:
                list<T>::_node* ptr;

            public:
                class IteratorOutOfBoundsException : public std::exception {
                    const char* what() const noexcept override {
                        return "iterator out of bounds";
                    }
                };

                iterator(list<T>::_node* p = nullptr) :
                ptr(p) { }

                T& operator*() const {
                    return ptr->payload; 
                }

                list<T>::iterator& operator++() { 
                    if(ptr->next == nullptr){
                        throw list<T>::iterator::
                        IteratorOutOfBoundsException();
                    }
                    ptr = ptr->next; 
                    return *this; 
                }

                bool operator==
                (iterator& other) const {
                    return this->ptr == other.ptr;
                }

                bool operator!=
                (iterator& other) const {
                    return this->ptr != other.ptr;
                }
        };

        list() : head(nullptr), size(0) { }

        list(T element){
            this->head = new list<T>::_node;
            head->payload = element;
            head->next = nullptr;
            this->size = 1;
        }

        
        list(T* array, unsigned int size){
            for(unsigned int i = 0; i < size; i++){
                this->push_back(array[i]);
            }
        }

        list(list<T>& prev_list) { 
            if (prev_list.head == nullptr) {
                return;
            }

            list<T>::_node* ptr = prev_list.head;
            while (ptr != nullptr) {
                this->push_back(ptr->payload);
                ptr = ptr->next;
            }
        }

        ~list(){
            list<T>::_node* current = this->head;
            while (current != nullptr) {
                list<T>::_node* next = current->next;
                delete current;
                current = next;
            }
            this->head = nullptr;
            this->size = 0;
        }

        void push_front(T value) {
            list<T>::_node* n = new list<T>::_node{value, head};
            this->head = n;
            this->size++;
        }

        void push_back(T element){
            if(head == nullptr){
                this->head = new list<T>::_node(element);
                this->size = 1;
                return;
            }

            list<T>::_node* iterator = this->head;
            while(iterator->next != nullptr){
                iterator = iterator->next;
            }

            iterator->next = new _node(element);
            this->size++;
        }

        T pop_back(){
            if(this->head == nullptr){
                throw list<T>::EmptyListException();                
            }
                    
            if (this->head->next == nullptr) {
                T value = head->payload;
                delete this->head;
                this->head = nullptr;
                this->size = 0;
                return value;
            }

            list<T>::_node* prev = this->head;
            list<T>::_node* current = this->head->next;

            while(current->next != nullptr){
                prev = current;
                current = current->next;
            }

            T value = current->payload;

            delete current;
            prev->next = nullptr;

            this->size--;

            return value;
        }


        T pop_front(){
            if(this->head == nullptr){
                throw list<T>::EmptyListException();                
            }

            if(this->head->next == nullptr){
                T value = this->head->payload;

                delete head;
                this->size = 0;

                return value;
            }

            list<T>::_node *ptr = this->head->next;

            T value = this->head->payload;

            delete this->head;

            this->head = ptr;

            size--;

            return value;
        }

        T peek_front() const {
            if(this->head == nullptr){
                throw list<T>::EmptyListException();   
            }

            return this->head->payload;
        }

        T peek_back() const {
            if(this->head == nullptr){
                throw list<T>::EmptyListException();                
            }

            list<T>::_node* iterator = this->head;

            while(iterator->next != nullptr){
                iterator = iterator->next;
            }

            return iterator->payload;
        }

        void insert(T element, unsigned int index) {
            if (index >= this->size){
                throw list<T>::OutOfRangeException();
            }

            if (index == 0) {
                this->push_front(element);
                return;
            } 

            if(index == this->size){
                this->push_back(element);
                return;
            }

            list<T>::_node* tmp = this->head;
            for (unsigned int i = 0; i < index - 1; ++i) {
                tmp = tmp->next;
            }

            list<T>::_node* new_node = new list<T>::_node(element, tmp->next);
            tmp->next = new_node;

            this->size++;
        }

        T at(unsigned int index) const {
            if (index >= this->size) {
                throw list<T>::OutOfRangeException();
            }

            if(this->head == nullptr){
                throw list<T>::EmptyListException();
            }

            list<T>::_node* tmp = this->head;
            for (unsigned int i = 0; i < index; ++i) {
                tmp = tmp->next;
            }

            return tmp->payload;
        }

        void replace(T value, unsigned int index){
            if(this->head == nullptr){
                throw list<T>::EmptyListException();                
            }
        }

        T remove(unsigned int index) {
            if (index >= this->size) {
                throw list<T>::OutOfRangeException();
            }

            if(this->head == nullptr){
                throw list<T>::EmptyListException();
            }

            list<T>::_node* tmp = this->head;
            T removed_value;

            if (index == 0) {
                removed_value = tmp->payload;
                this->head = tmp->next;
                delete tmp;
            } else {
                list<T>::_node* prev = nullptr;
                for (unsigned int i = 0; i < index; i++) {
                    prev = tmp;
                    tmp = tmp->next;
                }

                removed_value = tmp->payload;
                prev->next = tmp->next;
                delete tmp;
            }

            this->size--;

            return removed_value; 
        }

        list<T>::iterator begin() const {
            return list<T>::iterator(this->head);
        }

        list<T>::iterator end() const {
            return list<T>::iterator();
        }

        list<T>::iterator last() const {
            if(this->head == nullptr){
                return list<T>::iterator();
            }

            list<T>::_node* ptr = this->head;

            while(ptr->next != nullptr){
                ptr = ptr->next;
            }

            return iterator(ptr);
                            
        }

        T* array(){
            if(this->head == nullptr){
                return nullptr;
            }

            T* arr = new T[this->size];
            int i = 0;
            list<T>::_node *ptr = this->head;

            while(ptr->next != nullptr){
                arr[i++] = ptr->payload;
                ptr = ptr->next;
            }

            return arr;
        }

        void reverse(){
            list<T>::_node* prev = nullptr; 
            list<T>::_node* current = this->head;
            list<T>::_node* next = nullptr; 

            while (current != nullptr) {
                next = current->next;
                current->next = prev;
                prev = current;
                current = next;
            }

            this->head = prev;  
        }

        void clear() {
            list<T>::_node* current = this->head;
                    
            while (current != nullptr) {
                list<T>::_node* next = current->next;
                delete current;
                current = next;
            }
                    
            this->head = nullptr;
            this->size = 0;
        }

        unsigned int len() const {
            return this->size;
        }

        template <typename Predicate>
        void remove_if(Predicate pred) {
            for(int i = 0; i < this->len(); ) {
                if(pred(this->at(i))) {
                    this->remove_at(i);
                } else {
                    i++;
                }
            }
        }

};

#endif
