EXECUTABLE = bin/planning

INC_DIR = include
SRC_DIR = src
OBJ_DIR = obj

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

CXX = g++
CXXFLAGS = -fPIC -std=c++11 -O4 -DNDEBUG \
 -I$(INC_DIR) -I. -DARCH_K8 -Wno-deprecated \
 -DUSE_CBC -DUSE_CLP -DUSE_BOP -DUSE_GLOP
LDLIBS = -Llib -Llib64 -lprotobuf -lglog -lgflags -lCbcSolver -lCbc -lOsiCbc -lCgl -lClpSolver -lClp -lOsiClp -lOsi -lCoinUtils -lortools\
-Wl,-rpath,"\$$ORIGIN" \
-Wl,-rpath,"\$$ORIGIN/../lib64" \
-Wl,-rpath,"\$$ORIGIN/../lib" \
-lz -lrt -lpthread

$(EXECUTABLE): $(OBJECTS) 
	@echo  "\033[31m \nLinking all objects files: \033[0m"
	$(CXX) $(OBJECTS) $(LDLIBS) -o $@ 

-include $(OBJECTS:.o=.d)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo  "\033[31m \nCompiling $<: \033[0m"
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo  "\033[32m \nCreating $< dependency file: \033[0m"
	$(CXX) -std=c++11  -MM -I$(INC_DIR) -I. -LDLIBS $< > $(basename $@).d
	@mv -f $(basename $@).d $(basename $@).d.tmp
	@sed -e 's|.*:|$(basename $@).o:|' < $(basename $@).d.tmp > $(basename $@).d
	@rm -f $(basename $@).d.tmp

clean:
	@echo "\033[31mCleaning obj directory... \033[0m"
	@rm $(EXECUTABLE) -f $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d


rebuild: clean $(EXECUTABLE)

	
