/** Return index of element in a[nElements]; < 0 if not found. */
int
search_for_element(int a[], int nElements, int element)
{
  for(int i = 0; i < nElements;i++){
    if(a[i] == element) return i;
  }
  return -1;
}
