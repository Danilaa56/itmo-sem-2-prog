#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class CPoint{

};

template<typename DataType>
class CircularBuffer {

public:

    class MyIterator : public std::iterator<std::random_access_iterator_tag, DataType> {
    public:
        explicit MyIterator(CircularBuffer<DataType> const* b, int capacity): buffer(b), lapSize(capacity){}

        MyIterator& operator++(){
            position++;
            if(position == lapSize){
                position = 0;
                lap++;
            }
            return *this;
        }
        MyIterator& operator +=(int num){
            position += num;
            if(position >= lapSize){
                lap += num / lapSize;
                position = num % lapSize;
            }
            return *this;
        }
        MyIterator operator +(int num){
            MyIterator newOne = *this;
            newOne += num;
            return newOne;
        }
        MyIterator& operator--(){
            position--;
            if(position == -1){
                --lap;
                position = lapSize - 1;
            }
            return *this;
        }
        MyIterator& operator -=(int num){
            position -= num;
            if(position < 0){
                while(position < 0){
                    position += lapSize;
                    --lap;
                }
            }
            return *this;
        }
        MyIterator operator -(int num){
            MyIterator newOne = *this;
            newOne -= num;
            return newOne;
        }

        bool operator <(MyIterator const& other) const{
            if(lap < other.lap)
                return true;
            if(lap > other.lap)
                return false;
            return position < other.position;
        }
        bool operator ==(MyIterator const& other) const{
            return lap == other.lap && position == other.position;
        }
        bool operator >(MyIterator const& other) const{
            if(lap > other.lap)
                return true;
            if(lap < other.lap)
                return false;
            return position > other.position;
        }
        bool operator >=(MyIterator const& other) const{
            return operator >(other) || operator==(other);
        }
        bool operator <=(MyIterator const& other) const{
            return operator<(other) || operator==(other);
        }
        bool operator !=(MyIterator const& other) const{
            return !(operator==(other));
        }

        MyIterator& operator = (MyIterator const& other){
            position = other.position;
            lap = other.lap;
            lapSize = other.lapSize;
            buffer = other.buffer;
            return *this;
        }

        DataType& operator *(){
            return buffer->buffer[position];
        }

        int operator -(MyIterator const& other) const{
            return lap*lapSize + position - other.lap*other.lapSize - other.position;
        }

    public:
        int position = 0;
        int lap = 0;
        int lapSize = 0;

        CircularBuffer const* buffer;
    };

    MyIterator beginIter, endIter;

    explicit CircularBuffer(size_t _capacity) : capacity(_capacity), beginIter(this, _capacity), endIter(this, _capacity) {
        buffer = (DataType*) malloc(sizeof(DataType) * _capacity);
    }

    ~CircularBuffer(){
        free(buffer);
    }

    void push_back(DataType const& value){
        *endIter = value;
        ++endIter;
        endIter.lap--;
        if(endIter > beginIter)
            ++beginIter;
        endIter.lap++;
    }

    void push_front(DataType const& value){
        if(beginIter.position == endIter.position && beginIter.lap != endIter.lap)
            --endIter;
        --beginIter;
        *beginIter = value;
    }

    void erase_back(){
        --endIter;
        if(endIter < beginIter)
            beginIter = endIter;
    }

    void erase_front(){
        ++beginIter;
        if(endIter < beginIter)
            endIter = beginIter;
    }

    MyIterator begin(){
        return MyIterator(beginIter);
    }

    MyIterator end(){
        return MyIterator(endIter);
    }

    void clear(){
        beginIter.position = 0;
        beginIter.lap = 0;
        endIter.position = 0;
        endIter.lap = 0;
    }

    DataType& operator[](int index){
        return buffer[(beginIter.position + index) % capacity];
    }

    void resize(int newCapacity){
        if(newCapacity == capacity)
            return;
        auto newBuffer = (DataType*) malloc(sizeof(DataType)*newCapacity);
        int length = endIter - beginIter;
        if(newCapacity >= length) {
            int index = 0;
            for(auto iter = begin(); iter < endIter; ++iter){
                newBuffer[index++] = *iter;
            }
            endIter.position = index;
            endIter.lap = 0;
        } else {
            auto iter = MyIterator(endIter);
            for(int index = newCapacity - 1; index >= 0; index--){
                --iter;
                newBuffer[index] = *iter;
            }
            endIter.position = 0;
            endIter.lap = 1;
        }
        free(buffer);
        buffer = newBuffer;
        beginIter.position = 0;
        beginIter.lap = 0;
        beginIter.lapSize = newCapacity;
        endIter.lapSize = newCapacity;
    }

private:
    size_t capacity;
    DataType* buffer = nullptr;
};

template<typename DataType>
std::ostream& operator << (std::ostream& os, CircularBuffer<DataType>& buffer) {
    for(auto iter = buffer.begin(); iter < buffer.end(); ++iter)
        os << *iter << " ";
    return os;
}

int main(){

    CircularBuffer<int> pool(8);

    for(int i = 0; i < 11; i++){
        pool.push_back(i*5 + 10);
        std::cout << pool << "\n";
    }

    pool.clear();

    for(int i = 0; i < 11; i++){
        pool.push_front(i*5 + 10);
        std::cout << pool << "\n";
    }

    pool.resize(4);
    std::cout << pool << "\n";

    pool.resize(10);
    pool.clear();
    for(int i = 0; i < 5; i++){
        pool.push_back(i*2 + 3);
        pool.push_back(i*4 + 1);
    }
    std::cout << pool << "\n";

    std::sort(pool.begin(), pool.end());
    std::cout << pool << "\n";
}
