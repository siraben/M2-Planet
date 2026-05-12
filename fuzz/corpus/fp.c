typedef int (*F)(int); int id(int x){return x;} int main(){F f=id; return f(1);}
