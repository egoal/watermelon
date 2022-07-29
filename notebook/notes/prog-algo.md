```cpp
// quick sort
void quick_sort(std::vector<int>& vec, std::size_t left, std::size_t right){
    if(left< right){
        std::size_t i{left}, j{right}, val{vec[left]};
        while(i< j){
            // <-- j
            while(i< j && vec[j]>= val) --j;
            if(i< j) vec[i++] = vec[j];
            // i -->
            while(i< j && vec[i]< val) ++i;
            if(i< j) vec[j--] = vec[i];
        }
        
        vec[i] = val;
        quick_sort(vec, left, j- 1);
        quick_sort(vec, j+1 , right);
    }
}
```

