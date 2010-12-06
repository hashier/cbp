#include"ExprNodes.h"
#include"ProgramNodes.h"

#include <sstream>

void File::gen(CodeGen* out) {

    std::string filename = out->getInputFileName();
    size_t found;
    found=filename.find_last_of("/\\");
    filename = filename.substr(found+1);


#ifdef APPLE
#endif

#ifdef UNIX
    (*out) << Command(".file \"")(noSpace, filename)(noSpace, "\""); //Dateiname einfügen
#endif

#ifdef WIN32
    (*out) << Command(".file \"")(noSpace, filename)(noSpace, "\"");  //Dateiname einfügen
#endif

    {
        std::list<Variable*>::iterator it;
        for ( it = variables.begin() ; it != variables.end(); it++ ) {
            (*it)->gen(out);
        }
    }

#ifdef APPLE
    (*out) << Command(".text");
#endif

#ifdef UNIX
    (*out) << Command(".text");
#endif

#ifdef WIN32
    (*out) << Command(".text");
#endif


    {
        std::list<Function*>::iterator it;
        for ( it = functions.begin() ; it != functions.end(); it++ ) {
            (*it)->gen(out);
        }
    }

#ifdef APPLE
    (*out) << Command(".subsections_via_symbols");
#endif

#ifdef UNIX
    (*out) << Command("");
#endif

#ifdef WIN32
    (*out) << Command("");
#endif

}

void ForLoop::gen(CodeGen* out) {

    int offset_it     = iterator->getMemoryOffset(); 
    Label label_repeat = out->newMark("repeat");
    Label label_exit   = out->newMark("exit");

    //-----------------------------------------------------------------
    //set iterator to init
    init_value->gen(out);                                        //get inital value to eax
    *out << Command("mov")("%eax,")(offset_it, "(%esp)");        //mov eax to iterator

    //-----------------------------------------------------------------
    //set label
    *out << label_repeat;

    //-----------------------------------------------------------------
    //compare 1
    final_value->gen(out);                                      //get final value to eax
    *out << Command("cmp")(offset_it,"(%esp),")("%eax");        //compare x(%esp), eax [iterator, final value]

    //-----------------------------------------------------------------
    //compare 2 (alternative)
    *out << Command("push")("%ebx");                            //save ebx
    *out << Command("push")("%eax");                            //save eax
    final_value->gen(out);                                      //get final value to eax
    *out << Command("mov")("%eax,")("%ebx");                    //mov eax to ebx
    *out << Command("mov")(offset_it,"(%esp),")("%eax");        //mov iterator to eax
    *out << Command("cmp")("%eax,")("%ebx");                    //compare eax, ebx [iterator, final value]
    *out << Command("pop")("%eax");                             //restore eax
    *out << Command("pop")("%ebx");                             //restore ebx

    //-----------------------------------------------------------------
    //jump if exit
    *out << Command("je")(label_exit);

    //-----------------------------------------------------------------
    //write body
    body->gen(out);                                             //write body code

    //-----------------------------------------------------------------
    //increment iterator
    if(step==NULL)
    {
         *out << Command("push")("%eax");                      //save eax
         *out << Command("mov")(offset_it,"(%esp),")("%eax");  //mov iterator to eax
         *out << Command("inc")("%eax");                       //eax++
         *out << Command("mov")("%eax,")(offset_it,"(%esp)");  //mov eax to iterator
         *out << Command("pop")("%eax");                       //restore eax
    }else{
         //TODO
    }

    //-----------------------------------------------------------------
    //repeat
    *out << Command("jmp")(label_repeat);                      //jump to body begin

    //-----------------------------------------------------------------
    //set label
    *out << label_exit;
}
void IfElse::gen(CodeGen* out) {
    Label label_else = out->newMark("else"); 
    Label label_exit = out->newMark("exit");

    //-----------------------------------------------------------------
    condition->gen(out);                                     //get condition
    *out << Command("cmp")("$0,")("%eax");  	             //compare 0, %eax [false,condition]
    //-----------------------------------------------------------------
    //jump to else
    *out << Command("je")(label_else);

    //-----------------------------------------------------------------
    //write then
    then->gen(out);                                          //write body code

    //-----------------------------------------------------------------
    //jump to exit
    *out << Command("jmp")(label_exit);

    //-----------------------------------------------------------------
    //set label
    *out << label_else;

    //-----------------------------------------------------------------
    //write then
    if(otherwise)
        otherwise->gen(out);                                      //write body code

    //-----------------------------------------------------------------
    //set label
    *out << label_exit;
}

void SwitchCase::gen(CodeGen* out) {
    Label exitLabel = out->newMark("switchExit");
    
    // execute condition -> result in %eax
    which->gen(out);
    
    std::list<Label> jumpLabels;
    std::stringstream convert;
    
    // generate comparison for each possibility
	std::list<Case*>::const_iterator caseIterator = cases->begin();
	std::list<Case*>::const_iterator caseEnd = cases->end();
	for(; caseIterator != caseEnd; ++caseIterator){
	    int condition = (*caseIterator)->condition->val();
	    
	    convert << "case" << condition;
	    jumpLabels.push_back(out->newMark(convert.str()));
	    convert.str(""); convert.clear();
	    
	    *out << Command("cmpl")("$",condition)(", %eax");
	    *out << Command("je")(jumpLabels.back());
	}
	
	// Jump to exit if no comparsion matched.
	// If the comparison for the first case is moved to the back
	// this extra jump instruction can be removed.
	*out << Command("jmp")(exitLabel);
	
	// generate jump labels and corresponding code
	// for statements
	caseIterator = cases->begin();
	std::list<Label>::const_iterator labelIterator = jumpLabels.begin();
	for(; caseIterator != caseEnd; ++caseIterator, ++labelIterator){
	    // jump label
	    *out << *labelIterator;
	    // statement for this case
	    (*caseIterator)->action->gen(out);	    
	    // jump to exit after execution
	    *out << Command("jmp")(exitLabel);
	}
	
	// exit label
	*out << exitLabel;
}

void WhileLoop::gen(CodeGen* out) {

    Label label_repeat = out->newMark("repeat");
    Label label_exit   = out->newMark("exit");

    //-----------------------------------------------------------------
    //set label
    *out << label_repeat;

    //-----------------------------------------------------------------
    condition->gen(out);                                     //get condition
    *out << Command("cmp")("$0,")("%eax");                   //compare $0, eax [false,condition]
    //-----------------------------------------------------------------
    //jump if exit
    *out << Command("je")(label_exit);

    //-----------------------------------------------------------------
    //write body
    body->gen(out);                                           //write body code

    //-----------------------------------------------------------------
    //repeat
    *out << Command("jmp")(label_repeat);                   //jump to check condition

    //-----------------------------------------------------------------
    //set label
    *out << label_exit;
}

void Variable::gen(CodeGen* out) {
    // Declare global variable
    *out << Command(".comm")(identifier)(",",type->getSize())(",8");
}

void Local::gen(CodeGen* out) {
    // Declare local variable
    // Seems nothing is needed for local var declaration. It lives on the stack.
}

void Return::gen(CodeGen* out) {
    //*out << "push ebp" << std::endl;
    //*out << "mov esp, ebp" << std::endl;
    //*out << "sub esp, 16" << std::endl;

    if(expr != NULL)
        expr->gen(out);                                                 // write expression
    else
        *out << Command("mov")("$0xdeadbeef,")("%eax");                 // mov 0xdeadbeef to eax

    *out << Command("mov")("%ebp,")("%esp");                            // set pointers of the stack back
    *out << Command("pop")("%ebp");
    *out << Command("ret");                                             // return
}

int IfElse::calcStackOffset(int offset) {
    offset += then->calcStackOffset(offset);
    if(otherwise)
        offset += otherwise->calcStackOffset(offset);
    // Return the offset
    return offset;
}
int WhileLoop::calcStackOffset(int offset) {
    offset += body->calcStackOffset(offset);
    // Return the offset
    return offset;
}
int ForLoop::calcStackOffset(int offset) {
    offset += body->calcStackOffset(offset);
    // Return the offset
    return offset;
}
