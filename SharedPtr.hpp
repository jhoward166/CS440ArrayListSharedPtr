#include <atomic>
#include <iostream>

namespace cs540{
	class Capsul{
        public:
			std::atomic<int> refCount = {1};
            //int refCount=1;

            Capsul()
			//:refCount{}
			{
			}
            virtual ~Capsul(){
            }
    };

	template <typename U> class Preserve: public Capsul {
		public:
			U * deleteThisObject;

			Preserve(U * delObj)
			:deleteThisObject{delObj}
			{}

			~Preserve(){
                if(deleteThisObject != nullptr){
    				delete deleteThisObject;
                }
			}
	};

    template <typename T> class SharedPtr {
        public:
            T * object;
            Capsul * refContainer;
        public:
            //constructors
            SharedPtr()
                :object{nullptr},
				refContainer{nullptr}
                { }

            template <typename U> explicit SharedPtr(U * newObj)
                :object{newObj},
                refContainer{new Preserve<U>(newObj)}
                { }

            SharedPtr(const SharedPtr& sp)
                :object{sp.object},
                refContainer{sp.refContainer}
                {
                    if(sp.object != nullptr){
						if(refContainer != nullptr){
                            //std::cout << refContainer->refCount << std::endl;
		                    refContainer->refCount++;

						}
                    }
                }

            template <typename U> SharedPtr(const SharedPtr<U>& sp)
                :object{sp.object},
				refContainer{sp.refContainer}
                {
                    if(sp.object != nullptr){
						if(refContainer != nullptr){
		                    refContainer->refCount++;
						}
                    }

                }

            SharedPtr(SharedPtr&& sp)
                :object{sp.object},
				refContainer{sp.refContainer}
                {
                    sp.object = nullptr;
                    sp.refContainer = nullptr;
                }

            template <typename U> SharedPtr(SharedPtr<U>&& sp)
                :object{sp.object},
				refContainer{sp.refContainer}
                {
                    sp.object = nullptr;
                    sp.refContainer = nullptr;
                }

            ~SharedPtr(){
                if(refContainer != nullptr){
					if(--(refContainer->refCount) == 0){
						delete refContainer;
						refContainer = nullptr;
						object = nullptr;
                	}
				}
			}

            //methods
            T *get() const{
                return object;
            }

            template <typename U> void reset(U *p){
				if(refContainer != nullptr){
                    if(--(refContainer->refCount) == 0){
		                delete refContainer;
						refContainer = nullptr;
						object = nullptr;
		            }
				}
                object = p;
                refContainer = new Preserve<U>(p);
            }

            void reset(std::nullptr_t){
				if(refContainer != nullptr){
					if(--(refContainer->refCount) == 0){
						delete refContainer;
						refContainer = nullptr;
						object = nullptr;
					}
				}
                object = nullptr;
                refContainer = nullptr;
            }

            //operators
			SharedPtr& operator=(SharedPtr&& sp){
				if(this != &sp){
					if(refContainer != nullptr){
						if(--(refContainer->refCount) == 0){
							delete refContainer;
							refContainer = nullptr;
							object = nullptr;
						}
					}
					refContainer = sp.refContainer;
					object = sp.object;
					sp.object = nullptr;
					sp.refContainer = nullptr;
				}
				return this;
			}

			template <typename U> SharedPtr& operator=(SharedPtr<U>&& sp){
				if(refContainer != nullptr){
					if(--(refContainer->refCount) == 0){
						delete refContainer;
						refContainer = nullptr;
						object = nullptr;
					}
				}
				refContainer = sp.refContainer;
				object = sp.object;
				sp.object = nullptr;
				sp.refContainer = nullptr;
				return this;
			}

			SharedPtr& operator=(const SharedPtr& sp){
				if(this != &sp){
					if(refContainer != nullptr){
						//--(refContainer->refCount);
						if(--(refContainer->refCount) == 0){
							delete refContainer;
							refContainer = nullptr;
							object = nullptr;
						}
					}
					refContainer = nullptr;
					object = nullptr;
					if(sp.refContainer != nullptr){
						sp.refContainer->refCount++;
						refContainer = sp.refContainer;
						object = sp.object;
						
					}
				}
				return *this;
			}

			template <typename U> SharedPtr& operator=(const SharedPtr<U>& sp){
				if(refContainer != nullptr){
					
					if(--(refContainer->refCount) == 0){
						delete refContainer;
                        refContainer = nullptr;
						object = nullptr;
					}
				}
				refContainer = nullptr;
				object = nullptr;
				if(sp.refContainer != nullptr){
					refContainer = sp.refContainer;
					object = sp.object;
					refContainer->refCount++;
				}
				return *this;
			}

			T& operator*() const{
				return *object;
			}

			T *operator->() const{
				return object;
			}

			explicit operator bool() const{
				if(this != nullptr && object != nullptr){
					return true;
				}else{
					return false;
				}
			}

			//comparison operators
			template <typename U> friend bool operator==(const SharedPtr& ptr1, const SharedPtr<U>& ptr2){
				if(ptr1.object == ptr2.object){
					return true;
				}else{
					return false;
				}
			}

			friend bool operator==(const SharedPtr &ptr1, std::nullptr_t){
				if(ptr1.object == nullptr){
					return true;
				}else{
					return false;
				}
			}

			friend bool operator==(std::nullptr_t, const SharedPtr& ptr1){
				if(ptr1.object == nullptr){
					return true;
				}else{
					return false;
				}
			}

			template <typename U> friend bool operator!=(const SharedPtr& ptr1, const SharedPtr<U>& ptr2){
				if(ptr1.object == ptr2.object){
					return false;
				}else{
					return true;
				}
			}

			friend bool operator!=(const SharedPtr& ptr1, std::nullptr_t){
				return ptr1.object != nullptr;
			}

			friend bool operator!=(std::nullptr_t, const SharedPtr& ptr1){

				if(ptr1.object == nullptr){
					return true;
				}else{
					return false;
				}
			}

    };
}
