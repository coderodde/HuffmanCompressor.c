valgrind --tool=memcheck \
  --leak-check=full \
  --show-leak-kinds=all \
  --errors-for-leak-kinds=all \
  --track-origins=yes \
  --num-callers=50 \
  --error-exitcode=42 \
  ./huffc -d huffc.huf huffc.dcmp

