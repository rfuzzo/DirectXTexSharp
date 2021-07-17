#pragma once

namespace CLI {

    template<class T>
    public ref class ManagedObject {
        protected:
            T* m_instance_;
        public:
            ManagedObject(T* instance) : m_instance_(instance){

            	
            }
    	
		
            virtual ~ManagedObject() {
                if (m_instance_ != nullptr) {
                    delete m_instance_;
                }
            }
            !ManagedObject() {
                if (m_instance_ != nullptr) {
                    delete m_instance_;
                }
            }
            T* get_instance() {
                return m_instance_;
            }
    };

    
}

