void HorizontalTopLine(int**& arr, int& index, int from, int to_step, int n) {
  for (int k = from; index <= n * n && k <= to_step; ++k) {
    arr[from][k] = index;
    ++index;
  }
}
void VerticalRightLine(int**& arr, int& index, int from, int to_step, int n) {
  for (int k = from; index <= n * n && k <= to_step; ++k) {
    arr[k][n - from] = index;
    ++index;
  }
}
void HorizontalBotLine(int**& arr, int& index, int to_step, int from, int n) {
  for (int k = to_step; index <= n * n && k >= from; --k) {
    arr[n - from - 1][k] = index;
    ++index;
  }
}
void VerticalLeftLine(int**& arr, int& index, int to_step, int from, int n) {
  int from_left = from - 1;
  for (int k = to_step; index <= n * n && k >= from; --k) {
    arr[k][from_left] = index;
    ++index;
  }
}
int** MakeSpiral(int n) {
  int** result = new int*[n];
  for (int index = 0; index < n; ++index) {
    result[index] = new int[n];
  }
  int from_left = 0;
  int from_top = 0;
  int to_bottom = n - 1;
  int to_right = n - 1;
  int index = 1;
  while (index <= n * n) {
    HorizontalTopLine(result, index, from_left, to_right, n);
    ++from_top;
    VerticalRightLine(result, index, from_top, to_bottom, n);
    --to_right;
    HorizontalBotLine(result, index, to_right, from_left, n);
    --to_bottom;
    VerticalLeftLine(result, index, to_bottom, from_top, n);
    ++from_left;
  }
  return result;
}