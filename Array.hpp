#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdexcept>
#include <assert.h>
#include <exception>

namespace cs540{

	class OutOfRange: public std::exception{
	};
	
	template <typename T, size_t... Dims> class Array;
	
	template <typename T, size_t Dim> class Array<T, Dim>{
		
		public:			
			using ValueType = T;
			size_t numItems = Dim;
			T data[Dim];

			class FirstDimensionMajorIterator{
				public:
					T * ptr;
					Array<T, Dim> * belongTo;
					size_t index = 0;
					T * absolute;
				public:
					FirstDimensionMajorIterator()
					:ptr{nullptr},
					absolute{nullptr}				
					{}
					
					FirstDimensionMajorIterator(Array<T, Dim> * belong, T * newPoint)
					:ptr{newPoint},
					belongTo{belong},
					absolute{newPoint}			
					{}
					
					FirstDimensionMajorIterator(const FirstDimensionMajorIterator &copyIter)
					:ptr{copyIter.ptr},
					belongTo{copyIter.belongTo},
					index{copyIter.index},
					absolute{copyIter.absolute}			
					{}
					
					FirstDimensionMajorIterator &operator=(const FirstDimensionMajorIterator &copyIter){
						ptr = copyIter.ptr;
						belongTo = copyIter.belongTo;
						index = copyIter.index;
						absolute = copyIter.absolute;
						return *this;
					}

					friend bool operator==(const FirstDimensionMajorIterator & iter1, const FirstDimensionMajorIterator & iter2){
						return iter1.ptr == iter2.ptr;
					}

					friend bool operator!=(const FirstDimensionMajorIterator & iter1, const FirstDimensionMajorIterator & iter2){
						return iter1.ptr != iter2.ptr;
					}

					FirstDimensionMajorIterator &operator++(){						
						if(ptr != nullptr){							
							index++;
							if(index < belongTo->numItems){
								ptr = &belongTo->data[index];
								absolute = &belongTo->data[index];		
							}else{
								ptr = (&belongTo->data[belongTo->numItems-1])+1;
								absolute = (&belongTo->data[belongTo->numItems-1])+1;
							}							
						}
						return *this;
					}

					FirstDimensionMajorIterator operator++(int){
						FirstDimensionMajorIterator temp(*this);
						if(ptr != nullptr){
							index++;
							if(index < belongTo->numItems){
								ptr = &belongTo->data[index];	
								absolute = &belongTo->data[index];		
							}else{
								ptr = (&belongTo->data[belongTo->numItems-1])+1;
								absolute = (&belongTo->data[belongTo->numItems-1])+1;
							}
						}
						return temp;
					}

					T &operator*() const{
						return *ptr;
					}
					
			};

		public:
			Array() = default;
			
			Array(const Array & copyArray){
				size_t i;
				for(i=0; i< numItems; i++){
					data[i] = copyArray.data[i];
				}
			}
			template <typename U>Array(const Array<U, Dim> & copyArray){
				size_t i;
				for(i=0; i< numItems; i++){
					data[i] = copyArray.data[i];
				}
			}

			Array &operator=(const Array & copyArray){
				if(this != &copyArray){
					size_t i;
					for(i=0; i< numItems; i++){
						data[i] = copyArray.data[i];
					}
				}
				return *this;
			}
			
			template <typename U> Array &operator=(const Array<U, Dim> & copyArray){
				size_t i;
				for(i=0; i< numItems; i++){
					data[i] = copyArray.data[i];
				}
				return *this;
			}

			T &operator[] (size_t thisLevelSize){
				if(thisLevelSize > numItems-1){
					throw OutOfRange (); 
				}
				return data[thisLevelSize];				
			}
			
			const T &operator[] (size_t thisLevelSize) const{
				if(thisLevelSize > numItems-1){
					throw OutOfRange (); 
				}
				return data[thisLevelSize];
			}

			~Array(){
			}

			FirstDimensionMajorIterator fmbegin(){
				FirstDimensionMajorIterator temp(this, &(data[0]));
				return temp;
			};

			FirstDimensionMajorIterator fmend(){
				FirstDimensionMajorIterator temp(this, &(data[0]));
				temp.ptr = (&data[numItems-1])+1;
				return temp;
			};

	};

	template <typename T, size_t First, size_t... Dims> class Array<T, First, Dims...>{
		
		public:
			using ValueType = T;
			size_t dimLen = First;
			Array<T, Dims...> children[First];

			class FirstDimensionMajorIterator{
				public: 
					size_t index = 0;
					typename Array<T, Dims...>::FirstDimensionMajorIterator lowerIter;			
					Array<T, Dims...> * ptr;					
					Array<T, First, Dims...> * belongTo;
					T * absolute;					

					FirstDimensionMajorIterator()
					:ptr{nullptr},
					belongTo{nullptr},
					absolute{nullptr}
					{}

					FirstDimensionMajorIterator(Array<T, First, Dims...> *belong, Array<T, Dims...> * newPoint)
					:ptr{newPoint},
					belongTo{belong}
					{
						lowerIter = newPoint->fmbegin();
						absolute = lowerIter.absolute;				
					}
					
					FirstDimensionMajorIterator(const FirstDimensionMajorIterator &copyIter)
					:index{copyIter.index},
					lowerIter{copyIter.lowerIter},
					ptr{copyIter.ptr},
					belongTo{copyIter.belongTo},
					absolute{copyIter.absolute}
					{}

					FirstDimensionMajorIterator &operator=(const FirstDimensionMajorIterator &copyIter){
						ptr =copyIter.ptr;
						index = copyIter.index;
						lowerIter = copyIter.lowerIter;
						belongTo = copyIter.belongTo;
						absolute = copyIter.absolute;
						return *this;	
					}

					friend bool operator==(const FirstDimensionMajorIterator & iter1, const FirstDimensionMajorIterator & iter2){
						if(iter1.absolute != nullptr && iter2.absolute != nullptr){
							return iter1.absolute == iter2.absolute;
						}else{
							return iter1.ptr == iter2.ptr;
						}
					}

					friend bool operator!=(const FirstDimensionMajorIterator & iter1, const FirstDimensionMajorIterator & iter2){
						if(iter1.absolute != nullptr && iter2.absolute != nullptr){
							return iter1.absolute != iter2.absolute;
						}else{
							return iter1.ptr != iter2.ptr;
						}
					}
	
					FirstDimensionMajorIterator &operator++(){						
						if(ptr != nullptr){
							lowerIter++;
							absolute = lowerIter.absolute;
							if(lowerIter.ptr == belongTo->children[index].fmend().ptr){
								index++;
								if(index < belongTo->dimLen){
									lowerIter = belongTo->children[index].fmbegin();
									ptr = &(belongTo->children[index]);
									absolute = lowerIter.absolute;
								}else{
									ptr = &(belongTo->children[belongTo->dimLen-1])+1;
									absolute = nullptr;
								}
							}
						}
						return *this;
					}

					FirstDimensionMajorIterator operator++(int){
						FirstDimensionMajorIterator temp(*this);
						if(ptr != nullptr){
							lowerIter++;
							absolute = lowerIter.absolute;
							if(lowerIter.ptr == belongTo->children[index].fmend().ptr){
								index++;
								if(index < belongTo->dimLen){
									lowerIter = belongTo->children[index].fmbegin();
									ptr = &(belongTo->children[index]);
									absolute = lowerIter.absolute;
								}else{
									ptr = &(belongTo->children[belongTo->dimLen-1])+1;
									absolute = nullptr;
								}
							}
						}
						return temp;
					}

					T &operator*() const{
						return *lowerIter;
					}
					
			};
		
		public:
			Array() = default;
			
			Array(const Array &arr){
				int i;				
				for(i=0; i < dimLen; i++){
					children[i] = arr.children[i];
				}
			}

			template <typename U> Array(const Array<U,First, Dims...> &arr){
				int i;				
				for(i=0; i < dimLen; i++){
					children[i] = arr.children[i];
				}
			}

			Array<T, First, Dims...>& operator=(const Array<T, First, Dims...>& arr){
				if(this != &arr){
					size_t i;
					for(i=0; i< dimLen; i++){
						children[i] = arr.children[i];
					}
				}
				return *this;
			}

			template<typename U> Array<T,First, Dims...>& operator=(const Array<U, First, Dims...>& arr){
				size_t i;
				for(i=0; i< dimLen; i++){
					children[i] = arr.children[i];
				}
				return *this;
			}
			
			Array<T, Dims...> &operator[] (size_t thisLevelSize){
				if(thisLevelSize > dimLen-1){
					throw OutOfRange (); 
				}
				return children[thisLevelSize];				
			}
			
			const Array<T, Dims...> &operator[] (size_t thisLevelSize) const{
				
				if(thisLevelSize > dimLen-1){
					throw OutOfRange (); 
				}
				return children[thisLevelSize];
			}

			FirstDimensionMajorIterator fmbegin(){				
				FirstDimensionMajorIterator temp(this, &(children[0]));
				return temp;
			};

			FirstDimensionMajorIterator fmend(){
				FirstDimensionMajorIterator temp(this, &(children[dimLen-1])+1);
				return temp;
			};
			
	};

	
}
