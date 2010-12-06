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
    (*out) << "\t.file\t\"" << filename << "\"" << std::endl;                          //Dateiname einfügen
#endif

#ifdef WIN32
    (*out) << "\t.file\t\"" << filename << "\"" << std::endl;                           //Dateiname einfügen
#endif

    {
        std::list<Variable*>::iterator it;
        for ( it = variables.begin() ; it != variables.end(); it++ ) {
            (*it)->gen(out);
        }
    }

#ifdef APPLE
    (*out) << "\t.text" << std::endl;
#endif

#ifdef UNIX
    (*out) << "\t.text" << std::endl;
#endif

#ifdef WIN32
    (*out) << "\t.text" << std::endl;
#endif


    {
        std::list<Function*>::iterator it;
        for ( it = functions.begin() ; it != functions.end(); it++ ) {
            (*it)->gen(out);
        }
    }

#ifdef APPLE
    (*out) << "\t.subsections_via_symbols" << std::endl;
#endif

#ifdef UNIX
    (*out) << "" << std::endl;
#endif

#ifdef WIN32
    (*out) << "" << std::endl;
#endif

}

void ForLoop::gen(CodeGen* out) {

    int offset_it     = iterator->getMemoryOffset(); 
    Mark label_repeat = out->newMark("repeat");
    Mark label_exit   = out->newMark("exit");

    //-----------------------------------------------------------------
    //set iterator to init
    init_value->gen(out);                                        //get inital value to eax
    *out << "mov %eax, " << offset_it << "(%esp)" << std::endl;  //mov eax to iterator

    //-----------------------------------------------------------------
    //set label
    *out << ".L" << label_repeat << ":" << std::endl;

    //-----------------------------------------------------------------
    //compare 1
    final_value->gen(out);                                      //get final value to eax
    *out << "cmp " << offset_it << "(%esp), %eax" << std::endl; //compare x(%esp), eax [iterator, final value]

    //-----------------------------------------------------------------
    //compare 2 (alternative)
    *out << "push %ebx" << std::endl;                           //save ebx
    *out << "push %eax" << std::endl;                           //save eax
    final_value->gen(out);                                      //get final value to eax
    *out << "mov %eax, %ebx" << std::endl;                      //mov eax to ebx
    *out << "mov " << offset_it << "(%esp), %eax" << std::endl; //mov iterator to eax
    *out << "cmp %eax, %ebx" << std::endl;                      //compare eax, ebx [iterator, final value]
    *out << "pop %eax" << std::endl;                            //restore eax
    *out << "pop %ebx" << std::endl;                            //restore ebx

    //-----------------------------------------------------------------
    //jump if exit
    *out << "je .L" << label_exit << std::endl;

    //-----------------------------------------------------------------
    //write body
    body->gen(out);                                             //write body code

    //-----------------------------------------------------------------
    //increment iterator
    if(step==NULL)
    {
         *out << "push %eax" << std::endl;                      //save eax
         *out << "mov " << offset_it << "(%esp), %eax" << std::endl;  //mov iterator to eax
         *out << "inc %eax" << std::endl;                       //eax++
         *out << "mov %eax, " << offset_it << "(%esp)" << std::endl;  //mov eax to iterator
         *out << "pop %eax" << std::endl;                       //restore eax
    }else{
         //TODO
    }

    //-----------------------------------------------------------------
    //repeat
    *out << "jmp .L" << label_repeat << std::endl;              //jump to body begin

    //-----------------------------------------------------------------
    //set label
    *out << ".L" << label_exit << ":" << std::endl;
}
void IfElse::gen(CodeGen* out) {
    Mark label_else = out->newMark("else"); 
    Mark label_exit = out->newMark("exit");

    //-----------------------------------------------------------------
    condition->gen(out);                                     //get condition
    *out << "cmp " << "$0, %eax" << std::endl;	             //compare 0, %eax [false,condition]
    //-----------------------------------------------------------------
    //jump to else
    *out << "je .L" << label_else << std::endl;

    //-----------------------------------------------------------------
    //write then
    then->gen(out);                                          //write body code

    //-----------------------------------------------------------------
    //jump to exit
    *out << "jmp .L" << label_exit << std::endl;

    //-----------------------------------------------------------------
    //set label
    *out << ".L" << label_else << ":" << std::endl;

    //-----------------------------------------------------------------
    //write then
    if(otherwise)
        otherwise->gen(out);                                      //write body code

    //-----------------------------------------------------------------
    //set label
    *out << ".L" << label_exit << ":" << std::endl;
}

void SwitchCase::gen(CodeGen* out) {
    Mark exitLabel = out->newMark("switchExit");
    
    // execute condition -> result in %eax
    which->gen(out);
    
    std::list<Mark> jumpLabels;
    std::stringstream convert;
    
    // generate comparison for each possibility
	std::list<Case*>::const_iterator caseIterator = cases->begin();
	std::list<Case*>::const_iterator caseEnd = cases->end();
	for(; caseIterator != caseEnd; ++caseIterator){
	    int condition = (*caseIterator)->condition->val();
	    
	    convert << "case" << condition;
	    jumpLabels.push_back(out->newMark(convert.str()));
	    convert.str(""); convert.clear();
	    
	    *out << '\t' << "cmpl $" << condition << ", %eax" << std::endl;
	    *out << '\t' << "je .L" << jumpLabels.back() << std::endl;
	}
	
	// Jump to exit if no comparsion matched.
	// If the comparison for the first case is moved to the back
	// this extra jump instruction can be removed.
	*out << '\t' << "jmp .L" << exitLabel << std::endl;
	
	// generate jump labels and corresponding code
	// for statements
	caseIterator = cases->begin();
	std::list<Mark>::const_iterator labelIterator = jumpLabels.begin();
	for(; caseIterator != caseEnd; ++caseIterator, ++labelIterator){
	    // jump label
	    *out << ".L" << *labelIterator << ':' << std::endl;
	    // statement for this case
	    (*caseIterator)->action->gen(out);	    
	    // jump to exit after execution
	    *out << '\t' << "jmp .L" << exitLabel << std::endl;
	}
	
	// exit label
	*out << ".L" << exitLabel << ':' << std::endl;
}

void WhileLoop::gen(CodeGen* out) {

    Mark label_repeat = out->newMark("repeat");
    Mark label_exit   = out->newMark("exit");

    //-----------------------------------------------------------------
    //set label
    *out << ".L" << label_repeat << ":" << std::endl;

    //-----------------------------------------------------------------
    condition->gen(out);                                     //get condition
    *out << "cmp " << "$0, %eax" << std::endl; //compare $0, eax [false,condition]
    //-----------------------------------------------------------------
    //jump if exit
    *out << "je .L" << label_exit << std::endl;

    //-----------------------------------------------------------------
    //write body
    body->gen(out);                                           //write body code

    //-----------------------------------------------------------------
    //repeat
    *out << "jmp .L" << label_repeat << std::endl;            //jump to check condition

    //-----------------------------------------------------------------
    //set label
    *out << ".L" << label_exit << ":" << std::endl;
}

void Variable::gen(CodeGen* out) {
    // Declare global variable
    *out << "\t.comm " << identifier << "," << type->getSize() << ",8" << std::endl;
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
        *out << "mov $0xdeadbeef, %eax" << std::endl;                   // mov 0xdeadbeef to eax

    *out << "mov %ebp, %esp" << std::endl;                              // set pointers of the stack back
    *out << "pop %ebp" << std::endl;
    *out << "ret" << std::endl;                                         // return
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
