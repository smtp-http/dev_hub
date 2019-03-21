
%.o:%.c
	@echo "CC $@"
	@$(CC) $(CFLAGS) -c -o $@ $<

%.o:%.cpp
	@echo "C++ $@" $(CXXFLAGS)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<
	
.%.d:%.c
	@echo "Generate $@"
	@$(CC) -MM $(CFLAGS) $(CXXFLAGS) $< > $@.depend
	@sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.depend > $@
	@rm -f $@.depend

.%.d:%.cpp
	@echo "Generate $@"
#	@echo "$(CXX) -MM $(CFLAGS) $(CXXFLAGS) $< > $@.depend"
	@$(CXX) -MM $(CFLAGS) $(CXXFLAGS) $< > $@.depend
	@sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.depend > $@
	@rm -f $@.depend

#sinclude $(OBJS:.o=.d)
sinclude $(patsubst %.o, .%.d, $(OBJS))
