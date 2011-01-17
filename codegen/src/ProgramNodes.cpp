#include"ExprNodes.h"
#include"Variables.h"
#include"ProgramNodes.h"
#include"Message.h"

#include <sstream>

Label Function::getMark(CodeGen* out) {
    switch(abi) {
        case Abi_c:
            gotMark = true;
            return Label((out->isWithUnderscore()?"_":"")+identifier);
        case Abi_default:
            if(!gotMark) {
                gotMark = true;
                mark = out->newMark((out->isWithUnderscore()?"_":"")+identifier, true);
            }
            return mark;
    }
    //bugfix: not all control paths return value
    return mark;
}

Statement* Function::getStatement()
{
    return statement;
}

void File::gen(CodeGen* out) {

    std::string filename = out->getInputFileName();
    size_t found;
    found=filename.find_last_of("/\\");
    filename = filename.substr(found+1);


#ifdef APPLE
#endif

#ifdef UNIX
    (*out) << (Directive(".file") << ' ' << '"' << filename << '"'); //Dateiname einfügen
#endif

#ifdef WIN32
    (*out) << (Directive(".file") << ' ' << '"' << filename << '"');  //Dateiname einfügen
#endif

    {
        std::list<Variable*>::iterator it;
        for ( it = variables.begin() ; it != variables.end(); it++ ) {
            (*it)->gen(out);
        }
    }

#ifdef APPLE
    (*out) << Directive(".text");
#endif

#ifdef UNIX
    (*out) << Directive(".text");
#endif

#ifdef WIN32
    (*out) << Directive(".text");
#endif


    {
        std::list<Function*>::iterator it;
        for ( it = functions.begin() ; it != functions.end(); it++ ) {
            (*it)->gen(out);
        }
    }

#ifdef APPLE
    (*out) << Directive(".subsections_via_symbols");
#endif

#ifdef UNIX
    (*out) << Directive("");
#endif

#ifdef WIN32
    (*out) << Directive("");
#endif

}

void ForLoop::gen(CodeGen* out) {
    *out << Message(DEBUG, "ForLoop::gen()", this);

    Address it_address = iterator->getAddress();
    Label label_repeat = out->newMark("repeat");
    Label label_exit   = out->newMark("exit");

    //magic nop to find for loop in code
    *out << Command("nop");
    *out << Command("nop");
    *out << Command("nop");
    *out << Command("nop");

//-----------------------------------------------------------------------------
//set iterator to init
    *out << Command("pushq")("%rax");                         //save rax
    init_value->gen(out);                                     //get init-value-expression to eax
    *out << Command("mov")("%eax")(it_address);               //copy init-value-expression to iterator
    *out << Command("popq")("%rax");                          //restore rax

//-----------------------------------------------------------------------------
//set label
    *out << label_repeat;

//-----------------------------------------------------------------------------
//compare
    *out << Command("pushq")("%rbx");                         //save ebx
    *out << Command("pushq")("%rax");                         //save eax
    final_value->gen(out);                                    ///get final-value-expression to eax
    *out << Command("mov")("%eax")("%ebx");                   //mov eax to ebx
    *out << Command("mov")(it_address)("%eax");               //mov iterator to eax
    *out << Command("cmp")("%eax")("%ebx");                   //compare eax, ebx [iterator, final value]
    *out << Command("popq")("%rax");                          //restore eax
    *out << Command("popq")("%rbx");                          //restore ebx

//-----------------------------------------------------------------------------
//jump if exit
    *out << Command("je")(label_exit);                        //exit jump

//-----------------------------------------------------------------------------
//write body
    body->gen(out);                                           //write body code

//-----------------------------------------------------------------------------
//increment iterator
    if(step==NULL){
         *out << Command("pushq")("%rax");

         *out << Command("mov")(it_address)("%eax");
         *out << Command("inc")("%eax");
         *out << Command("mov")("%eax")(it_address);

         *out << Command("popq")("%rax");
    }else{
         *out << Command("pushq")("%rax");
         *out << Command("pushq")("%rbx");

         step->gen(out);
         *out << Command("mov")(it_address)("%ebx");
         *out << Command("add")("%eax")("%ebx");
         *out << Command("mov")("%eax")(it_address);

         *out << Command("popq")("%rbx");
         *out << Command("popq")("%rax");
    }


//-----------------------------------------------------------------------------
//repeat
    *out << Command("jmp")(label_repeat);                      //jump to body begin

//-----------------------------------------------------------------------------
//set label
    *out << label_exit;
}
void IfElse::gen(CodeGen* out) {
    *out << Message(DEBUG, "IfElse::gen()", this);

    Label label_else = out->newMark("else");
    Label label_exit = out->newMark("exit");

    //-----------------------------------------------------------------
    condition->gen(out);                                     //get condition
    *out << Command("cmp")(0)("%eax");  	             //compare 0, %eax [false,condition]
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
    *out << Message(DEBUG, "SwitchCase::gen()", this);

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

	    *out << Command("cmpl")(condition)("%eax");
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
    *out << Message(DEBUG, "WhileLoop::gen()", this);

    Label label_repeat = out->newMark("repeat");
    Label label_exit   = out->newMark("exit");

    //-----------------------------------------------------------------
    //set label
    *out << label_repeat;

    //-----------------------------------------------------------------
    condition->gen(out);                                     //get condition
    *out << Command("cmp")(0)("%eax");                   //compare $0, eax [false,condition]
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
    *out << Message(DEBUG, "Variable::gen()", this);
    // Declare global variable
    *out << (Directive(".comm") << ' ' << identifier << "," << type->getSize() << ",8");
}

void Local::gen(CodeGen* out) {
    *out << Message(DEBUG, "Variable::gen()", this);
    // Declare local variable
    // Seems nothing is needed for local var declaration. It lives on the stack.
}

void Return::gen(CodeGen* out, bool outermost) {
    *out << Message(DEBUG, "Return::gen()", this);
    //*out << "push ebp" << std::endl;
    //*out << "mov esp, ebp" << std::endl;
    //*out << "sub esp, 16" << std::endl;

    if(expr != NULL)
        expr->gen(out);                                                 // write expression
    else
        *out << Command("mov")("$0xdeadbeef")("%eax");                 // mov 0xdeadbeef to eax

    if(!outermost) {
        *out << Command("leave");                                           // set pointers of the stack back
        *out << Command("ret");                                             // return
    }
}

int Local::calcStackOffset(int offset) {
    return var->setStackOffset(offset);
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
