#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/LegacyPassManager.h"
#define pfel(i) errs() << "" << ""
#define pf(i) errs() << ""
// #define pfel(i) errs() << i << "\n"
// #define pf(i) errs() << i
#define pfvar(i) errs() << "%" << i
#define getnamefo(Si) Si->getPointerOperand()->getName().str()
using namespace std;
// macro for print the Value" type of data we get from getoperand() function
#define pfop(operand)                                                          \
  operand->print(llvm::outs());                                                \
  llvm::outs();
#define pfopel(operand)                                                        \
  operand->print(llvm::outs());                                                \
  llvm::outs() << "\n";
// #define out(i) outfile << i
// #define outel(i) outfile << i<<"\n";

using namespace llvm;
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include <bits/stdc++.h>
#include <iostream>
ofstream outfile;
/*
identify itr function then update the value of the

Storeinst:
getoperand[0] //value this is either data or operand getvalue
getoperand[1] //destination pointer of the destination variable getpointer
operand

*/
std::string stripmod(std::string var) {
  if (var[0] == '%') {
    std::string var2(var.begin() + 1, var.end());
    return var2;
  } else
    return var;
}

std::string addmod(std::string var) {
  if (var.at(0) == '%')
    return var;
  std::string s = "%";
  return s.append(var);
}
void iterateME(Instruction *i) {
  unsigned numOperands = i->getNumOperands();

  for (unsigned ii = 0; ii < numOperands; ++ii) {
    Value *operand = i->getOperand(ii);
    // pfel(operand->print());
    pf("\nindex :");
    pfel(ii);
    pf("operand type:");
    pfopel(operand->getType());
    pf("Plain operand :");
    pfopel(operand);
    pfel("----------------------");
  }
}

std::string getStringFromInstruction(Instruction *inst) {
  // Instruction *inst = /* your instruction */;

  // Create a raw string output stream
  std::string str;
  llvm::raw_string_ostream stream(str);

  // Print the instruction to the string stream
  inst->print(stream);

  // Get the string representation of the instruction
  std::string instStr = stream.str();
  return instStr;
  // Print the string representation of the instruction
  // llvm::outs() << "Instruction: " << instStr << "\n";
}

std::string getresultOperand(Instruction *i) {
  std::string s;
  raw_string_ostream os(s);
  i->printAsOperand(os, false);
  os.flush();
  std::string instructionSubstr = os.str();
  return instructionSubstr;
}

std::string getVariableNamefromoperand(Value *i) {
  if (llvm::ConstantInt *constantint = llvm::dyn_cast<llvm::ConstantInt>(i)) {

    // pfel("value is constant");
    int value = constantint->getSExtValue();
    return std::to_string(value);
    ;
    pf(value);
    pf("\n");
  } else {
    // pfel("value is variable!!!");

    // Check if the stored value is an instruction
    if (Instruction *storedInstruction = dyn_cast<Instruction>(i)) {

      // Get the name of the variable from the instruction
      std::string variableName = storedInstruction->getName().str();

      // Print the name of the variable
      return getresultOperand(storedInstruction);
      pfel("HMMMMMMMM" << getresultOperand(storedInstruction));
      pfvar(variableName);
      pf("\n");
      return variableName;
      // outs() << "Variable name: " << variableName << "\n";
    } else {
      // The stored value is not an instruction
      pf("Stored value is not an instruction.\n");
    }
  }
}

void printMapValues(const std::map<std::string, std::string> &myMap) {
  pf("Values in the map:\n\t\t");

  for (const auto &pair : myMap) {
    pf("(" << pair.first << ":" << pair.second << ") ");
    // std::cout << pair.second << std::endl;
  }
}

std::vector<std::string> findSuccessors(Instruction *Inst) {
  std::vector<std::string> successorList;
  // if (!I)
  //   return;
  BasicBlock *BB = Inst->getParent();
  for (BasicBlock *Successor : successors(BB)) {
    // Do something with the successor
    // BasicBlock *Succ = Inst.getSuccessor(i);
    successorList.push_back(
        getStringFromInstruction(dyn_cast<Instruction>(Successor->begin())));
  }

  pf(getStringFromInstruction(Inst)
     << "sizeeeeeeeeeeEE:::::::::::" << successorList.size());
  return successorList;
}

void iterateVector(const std::vector<std::string> &vec) {
  for (const std::string &str : vec) {
    pf(str << " ");
    // std::cout << str << std::endl;
  }
  pf("\n");
}
bool isInteger(const std::string &str) {
  if (str.empty() ||
      ((!isdigit(str[0])) && (str[0] != '-') && (str[0] != '+'))) {
    return false;
  }

  char *p;
  strtol(str.c_str(), &p, 10);

  return (*p == 0);
}

int stringToInt(const std::string &str) {
  if (!isInteger(str)) {
    // Handle error or return a default value
    // std::cerr << "Invalid integer string: " << str << std::endl;
    return 0; // Default value
  }

  int intValue;
  std::istringstream iss(str);
  iss >> intValue;
  return intValue;
}

// take two stirgs and give out the meet of these two strings
std::string meetValue(std::string val1, std::string val2) {
  if (isInteger(val1) && isInteger(val2)) {
    // both are contant and equal therefore meet is contant value
    if (stringToInt(val1) == stringToInt(val2)) {
      return val1;
    } else {
      return "BOTTOM";
    }
  } else {
    if (val1 == "BOTTOM" || val2 == "BOTTOM") {
      return "BOTTOM";
    } else // either one is top
    {
      // return TOP in the val1/2 if false or returns constant value from val1/2
      return isInteger(val1) ? val1 : val2;
    }
  }
}

// To Do check this functionatlity
//  take two maps and calculates the meet of these two maps
//  x meet top = x
//  c1 meet c2 = bottom
//  x meet botom =bottom
std::map<std::string, std::string>
meet(std::map<std::string, std::string> tuple1,
     std::map<std::string, std::string> tuple2) {
  // pf("in meet");
  std::map<std::string, std::string> newtuple;

  for (auto i = tuple1.begin(); i != tuple1.end(); i++) {

    // same variable exits in both
    if (tuple2.find(i->first) != tuple2.end()) {
      // pf(i->first << ":" << i->second << "|-|" << tuple2.find(i->first))
      //     ->second;
      newtuple[i->first] = meetValue(i->second, tuple2.find(i->first)->second);
      // pfel("=" << newtuple[i->first]);

    } else {
      // copy variable as it is
      newtuple[i->first] = i->second;
    }
  }
  for (auto i = tuple2.begin(); i != tuple2.end(); i++) {
    // if it doesnt exist add it as it is
    if (tuple1.find(i->first) == tuple1.end()) {
      newtuple[i->first] = i->second;
    }
  }

  return newtuple;
}

bool mapChanged(const std::map<std::string, std::string> &map1,
                const std::map<std::string, std::string> &map2) {
  if (map1.size() != map2.size()) {
    return true; // Maps have different sizes
  }

  for (const auto &pair : map1) {
    auto it = map2.find(pair.first);
    if (it == map2.end() || it->second != pair.second) {
      return true; // Element not found in map2 or values are different
    }
  }

  return false; // Maps are equal
}

void printQueue(std::queue<std::string> gq) {
  std::queue<std::string> g = gq;
  while (!g.empty()) {
    // cout << '\t' << g.front();
    pf(g.front() << " \n");
    g.pop();
  }
  // cout << '\n';
  pf("\n");
}

// function takes input as instruction and also map of the current point
// std::map<std::string,Instruction *> getInstructionFromString;

std::map<std::string, std::string>
transferFunc(std::string curInstructionStr,
             std::map<std::string, std::string> curMap,
             std::map<std::string, Instruction *> getInstructionFromString,
             Instruction *ist) {
  // pf("\ngot ins is" << *ist << "\n");

  // need to update the curMap values in this one first;
  std::map<std::string, std::string> newMap;
  for (const auto &pair : curMap) {
    newMap.insert(pair);
  }
  // Instruction *inst=getInstructionFromString[curInstructionStr];
  // inst->getOpcode();

  switch (ist->getOpcode()) {
  default:
    break;

  case Instruction::Alloca: {
    pf("\t\tAlloca Instructino in "
       "FUNCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\n");
    pf(*ist);
    std::string instructionSubstr =
        getresultOperand(dyn_cast<Instruction>(ist));
    // instructionSubstr is always stripped of % by default so add %
    // instructionSubstr = addmod(instructionSubstr);
    pfel(" --> " << instructionSubstr << " = TOP");
    newMap[instructionSubstr] = "TOP";
    break;
  }
  case Instruction::Load: {
    pf("\t\tLoadddd Instruction\n");
    if (LoadInst *LI = dyn_cast<LoadInst>(ist)) {
      std::string s;

      raw_string_ostream os(s);
      LI->printAsOperand(os, false);
      os.flush();
      std::string instructionSubstr = os.str();
      pf(*LI);
      std::string variableName = LI->getPointerOperand()->getName().str();
      unsigned numOperands = LI->getNumOperands();
      pfel("-->" << instructionSubstr << " = " << variableName);

      // update from here
      // variable always has mode in load
      // instructionSubstr = addmod(instructionSubstr);

      // variableName=addmod(variableName);
      string destinationOpernad = instructionSubstr;
      if (isInteger(destinationOpernad)) {
        newMap[instructionSubstr] = destinationOpernad;

      } else {
        pf("checkkkkkkkkk " << curMap[addmod(variableName)] << " "
                            << addmod(variableName));
        newMap[instructionSubstr] = curMap[addmod(variableName)];
      }
    }
    break;
  }
  case Instruction::Store: {
    pf("\t\tStore Instruction\n");
    if (StoreInst *Si = dyn_cast<StoreInst>(ist)) {

      pfel(" " << *ist << " ");

      unsigned numOperands = Si->getNumOperands();

      std::string variableName = Si->getPointerOperand()->getName().str();
      Value *storedValue = Si->getValueOperand();

      // pfel("1st operand/variable name :");
      pf("-->");
      variableName = addmod(variableName);

      pf(variableName << " = ");
      string destinationOpernad = getVariableNamefromoperand(storedValue);
      pfel(destinationOpernad<<"?????????????????????????????");
      // val is int assign it to variable
      if (isInteger(destinationOpernad)) {
        newMap[variableName] = destinationOpernad;

      }else if(curMap.find(destinationOpernad)!=curMap.end())
      {
        pfel(variableName<<">>"<<curMap[destinationOpernad]);
        newMap[variableName]=curMap[destinationOpernad];
      }
       else {
        newMap[variableName] =
            meetValue(curMap[variableName], destinationOpernad);
        // newMap[variableName] =
        // curMap[getVariableNamefromoperand(storedValue)];
      }
    }
    break;
  }
  case Instruction::GetElementPtr: {
    pf("\t\t GET element\n");
    break;
  }

  case Instruction::Call: {
    pf("call instruciton");
    if (CallInst *Si = dyn_cast<CallInst>(ist)) {
      // iterateME(Si);

      Function *CalledFunc = Si->getCalledFunction();
      if (CalledFunc) {
          string FuncName = CalledFunc->getName().str();
          // outs()<<"hMM"<<FuncName<<"\n";
          if(FuncName=="printf"){
          // outs()<<"breaking stufff";
          break;
          }else
          {
            // outs()<<getVariableNamefromoperand(Si->getOperand(1))<<"IS BOTTTTTTTTT";
            newMap[getVariableNamefromoperand(Si->getOperand(1))] = "BOTTOM";

          }

          // return FuncName.equals("printf") || FuncName.equals("scanf");
      }



      // StringRef FuncName = Si->getName();
      

      // // outs()<<"calllllllll"<<FuncName;
      // iterateME(Si);
      // pf("operand value is\n");
      // pfop(Si->getOperand(1));pf("\n::::");
      // pf(getVariableNamefromoperand(Si->getOperand(1)));
    }
    break;
  }
  case Instruction::ICmp:
  case Instruction::Ret:
  case Instruction::Br:
  case Instruction::Invoke: {
    break;
  }

  // Binary Instructions
  case Instruction::Add:
  case Instruction::Sub:
  case Instruction::Mul:
  case Instruction::SDiv:
  case Instruction::UDiv:
  case Instruction::SRem:
  case Instruction::URem:
  case Instruction::Shl:
  case Instruction::LShr:
  case Instruction::AShr:
  case Instruction::And:
  case Instruction::Or:
  case Instruction::Xor: {
    pfel("\t\tBinary Instruction\n" << *ist);
    // iterateME(dyn_cast<Instruction>(i));
    string resultOperand, stringOp1, stringOp2;

    resultOperand = (getresultOperand(dyn_cast<Instruction>(ist)));
    stringOp1 = (getVariableNamefromoperand(ist->getOperand(0)));
    stringOp2 = (getVariableNamefromoperand(ist->getOperand(1)));
    pf(resultOperand << "= ");
    int op1, op2;

    if ((isInteger(stringOp1) && isInteger(stringOp2))) {
      op1 = stringToInt(stringOp1);
      op2 = stringToInt(stringOp1);
      switch (ist->getOpcode()) {
      default:
        break;
      case Instruction::Add: {
        newMap[resultOperand] = std::to_string(op1 + op2);
        break;
      }
      case Instruction::Sub: {
        newMap[resultOperand] = std::to_string(op1 - op2);
        break;
      }
      case Instruction::Mul: {
        newMap[resultOperand] = std::to_string(op1 * op2);
        break;
      }
      case Instruction::SDiv: {
        newMap[resultOperand] = std::to_string(op1 / op2);
        break;
      }
      case Instruction::UDiv: {
        newMap[resultOperand] = std::to_string(op1 / op2);
        break;
      }
      }
      // switch case execute operation
      // switch (ist->getOpcode()) {
      // default:
      //   break;
      // case Instruction::Add: {
      //   newMap[resultOperand] = std::to_string(op1 + op2);
      //   break;
      // }
      // case Instruction::Sub: {
      //   newMap[resultOperand] = std::to_string(op1 - op2);
      //   break;
      // }
      // case Instruction::Mul: {
      //   newMap[resultOperand] = std::to_string(op1 * op2);
      //   break;
      // }
      // case Instruction::SDiv: {
      //   newMap[resultOperand] = std::to_string(op1 / op2);
      //   break;
      // }
      // case Instruction::UDiv: {
      //   newMap[resultOperand] = std::to_string(op1 / op2);
      //   break;
      // }
      // case Instruction::SRem:
      // case Instruction::URem:
      // case Instruction::Shl:
      // case Instruction::LShr:
      // case Instruction::AShr:
      // case Instruction::And:
      // case Instruction::Or:
      // case Instruction::Xor:
      // }

    } else if (isInteger(curMap[stringOp1]) && isInteger(curMap[stringOp2])) {
      op1 = stringToInt((curMap[stringOp1]));
      op2 = stringToInt((curMap[stringOp2]));

      // // switch case execute operation
      switch (ist->getOpcode()) {
      default:
        break;
      case Instruction::Add: {
        newMap[resultOperand] = std::to_string(op1 + op2);

        break;
      }
      case Instruction::Sub: {
        newMap[resultOperand] = std::to_string(op1 - op2);
        break;
      }
      case Instruction::Mul: {
        newMap[resultOperand] = std::to_string(op1 * op2);
        break;
      }
      case Instruction::SDiv: {
        newMap[resultOperand] = std::to_string(op1 / op2);
        break;
      }
      case Instruction::UDiv: {
        newMap[resultOperand] = std::to_string(op1 / op2);
        break;
      }
        // case Instruction::SRem:
        // case Instruction::URem:
        // case Instruction::Shl:
        // case Instruction::LShr:
        // case Instruction::AShr:
        // case Instruction::And:
        // case Instruction::Or:
        // case Instruction::Xor:
      }
      // newMap[resultOperand] =
      // curMap[getVariableNamefromoperand(storedValue)];
    } else if (curMap[stringOp1] == "BOTTOM" || curMap[stringOp2] == "BOTTOM") {
      newMap[resultOperand] = "BOTTOM";

    } else if (isInteger(curMap[stringOp1]) && isInteger(stringOp2)) {
      pf("VALUE AND OPERANDS ARE HERE :"
         << "key" << stringOp1 << "val:" << curMap[stringOp1]);
      op1 = stringToInt(curMap[stringOp1]);
      op2 = stringToInt(stringOp2);
      switch (ist->getOpcode()) {
      default:
        break;
      case Instruction::Add: {
        newMap[resultOperand] = std::to_string(op1 + op2);

        break;
      }
      case Instruction::Sub: {
        newMap[resultOperand] = std::to_string(op1 - op2);
        break;
      }
      case Instruction::Mul: {
        newMap[resultOperand] = std::to_string(op1 * op2);
        break;
      }
      case Instruction::SDiv: {
        newMap[resultOperand] = std::to_string(op1 / op2);
        break;
      }
      case Instruction::UDiv: {
        newMap[resultOperand] = std::to_string(op1 / op2);
        break;
      }
        // case Instruction::SRem:
        // case Instruction::URem:
        // case Instruction::Shl:
        // case Instruction::LShr:
        // case Instruction::AShr:
        // case Instruction::And:
        // case Instruction::Or:
        // case Instruction::Xor:
      }

    } else if (isInteger(curMap[stringOp2]) && isInteger(stringOp1)) {
      pf("VALUE AND OPERANDS ARE HERE :"
         << "key" << stringOp2 << "val:" << curMap[stringOp2] << "ope2"
         << stringOp1);
      op2 = stringToInt(curMap[stringOp2]);
      op1 = stringToInt(stringOp1);
      switch (ist->getOpcode()) {
      default:
        break;
      case Instruction::Add: {
        newMap[resultOperand] = std::to_string(op1 + op2);

        break;
      }
      case Instruction::Sub: {
        newMap[resultOperand] = std::to_string(op1 - op2);
        break;
      }
      case Instruction::Mul: {
        newMap[resultOperand] = std::to_string(op1 * op2);
        break;
      }
      case Instruction::SDiv: {
        newMap[resultOperand] = std::to_string(op1 / op2);
        break;
      }
      case Instruction::UDiv: {
        newMap[resultOperand] = std::to_string(op1 / op2);
        break;
      }
        // case Instruction::SRem:
        // case Instruction::URem:
        // case Instruction::Shl:
        // case Instruction::LShr:
        // case Instruction::AShr:
        // case Instruction::And:
        // case Instruction::Or:
        // case Instruction::Xor:
      }

    } else {
      newMap[resultOperand] = "TOP";
    }
    // one more condition of cannot be evaluted

    // pf("\t\ttesttt\t\t");
    // pfopel(i->getOperand(0));

    pf(getVariableNamefromoperand(ist->getOperand(0)) << "= ");
    pf(getVariableNamefromoperand(ist->getOperand(1)) << "= ");
    pf("OUT=" << newMap[resultOperand]);

    break;
  }
  }

  return newMap;
}

namespace {
struct Hello : public FunctionPass {
  static char ID;
  Hello() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {

    // outfile.open("Myout.txt");
    // outfile << "Hello"
    //         << " ";

    // errs() << "Hello: ";
    // errs().write_escaped(F.getName()) << '\n';

    int linenumber = 0;
    // std::map<std::string, Instruction> linemap;

    // maintain worklist which is nothing but the instruction as string

    // every basic block has unique name and every basic block should have a
    // map to store the changes in the data values after map
    std::map<std::string, std::map<std::string, std::string>> valueTuple;

    std::map<std::string, std::vector<std::string>> successor;
    std::map<std::string, Instruction *> getInstructionFromString;
    // std::map<Instruction, int> tst;
    // std::map<Instruction *, std::map<std::string, int>> gmap;

    for (Function::iterator bb = F.begin(), e = F.end(); bb != e; bb++) {
      for (BasicBlock::iterator i = bb->begin(), i2 = bb->end(); i != i2; i++) {

        // add all the instruction in the worklist queue
        // worklist.push(getStringFromInstruction(dyn_cast<Instruction>(i)));
        // isChanged[getStringFromInstruction(dyn_cast<Instruction>(i))] =
        // false; visited[getStringFromInstruction(dyn_cast<Instruction>(i))] =
        // false; getInstructionFromString[getStringFromInstruction(
        //     dyn_cast<Instruction>(i))] = dyn_cast<Instruction>(i);
        // pf("HERE WE GO AGAIN \n\n\n\n\n"
        //    << *getInstructionFromString[getStringFromInstruction(
        //           dyn_cast<Instruction>(i))]);
        // pf("Getting the
        // instruction\n\n"<<getStringFromInstruction(getInstructionFromString[getStringFromInstruction(dyn_cast<Instruction>(i))]));
        // pfop(getInstructionFromString[getStringFromInstruction(dyn_cast<Instruction>(i))]);

        if (dyn_cast<Instruction>(i) == bb->getTerminator()) {
          // pf(getStringFromInstruction(dyn_cast<Instruction>(i))
          //    << "this is last node of block ");
          successor[getStringFromInstruction(dyn_cast<Instruction>(i))] =
              findSuccessors(dyn_cast<Instruction>(i));
          iterateVector(findSuccessors(dyn_cast<Instruction>(i)));
          pf("---\n");

        } else {
          // consecutive instruction in the basic block have next instruction as
          // successor
          std::vector<std::string> v;
          v.push_back(getStringFromInstruction(
              dyn_cast<Instruction>(i->getNextNode())));
          successor[getStringFromInstruction(dyn_cast<Instruction>(i))] = v;

          // pfel("\n"
          //      << getStringFromInstruction(dyn_cast<Instruction>(i)) <<
          //      "---->"
          //      << getStringFromInstruction(
          //             dyn_cast<Instruction>(i->getNextNode()))
          //      << " \n");
        }
      }
    }
    // pf("below is worklist\n\n");
    // printQueue(worklist);
    // pf("\n\n");
    bool Changed = false;
    int count = 0;

    // do while
    do {
      // if (count-- )
      //   break;

      std::queue<std::string> worklist;
      // std::queue<Instruction*> worklist2;
      // std::map<std::string, bool> isChanged;
      std::map<std::string, bool> visited;
      std::map<std::string, bool> inQ;
      Changed = false;

      for (Function::iterator bb = F.begin(), e = F.end(); bb != e; bb++) {
        for (BasicBlock::iterator i = bb->begin(), i2 = bb->end(); i != i2;
             i++) {

          // add all the instruction in the worklist queue
          if (worklist.empty())
            worklist.push(getStringFromInstruction(dyn_cast<Instruction>(i)));
          // isChanged[getStringFromInstruction(dyn_cast<Instruction>(i))] =
          // false;
          visited[getStringFromInstruction(dyn_cast<Instruction>(i))] = false;
          inQ[getStringFromInstruction(dyn_cast<Instruction>(i))] = false;
          getInstructionFromString[getStringFromInstruction(
              dyn_cast<Instruction>(i))] = dyn_cast<Instruction>(i);
        }
      }

      pfel("\t\t\t\tITERATION" << worklist.empty() << "||");

      // run while worklist is not empty
      while (!worklist.empty()) {
        pfel("\n\n");

        std::string curInstructionStr = worklist.front();
        worklist.pop();
        // pfel("current instruction");
        // pfel("--------------<" << curInstructionStr << ">");
        visited[curInstructionStr] = true;
        std::map<std::string, std::string> curMap =
            valueTuple[curInstructionStr];

        // get Instruction type from string of instruction
        // pf("retrived instruction"
        //    << *getInstructionFromString[curInstructionStr]);
        // Instruction *ist=getInstructionFromString[curInstructionStr]);

        // we pass instruction
        map<string, string> newMap =
            transferFunc(curInstructionStr, curMap, getInstructionFromString,
                         getInstructionFromString[curInstructionStr]);

        // if (mapChanged(newSuccessorMap, valueTuple[curr_successors[i]])) {
        if (mapChanged(newMap, curMap)) {
            pfel("map are changed");
            Changed = true;
          } else {
            pfel("NOT Changed");
          }

        pfel("print current map");
        printMapValues(curMap);

        pfel("\nupdated map after transfer function");
        printMapValues(newMap);
        // pfel("after map print+++++++++++++++++++++");
        // update the current map
        valueTuple[curInstructionStr] = newMap;

        vector<string> curr_successors = successor[curInstructionStr];

        for (size_t i = 0; i < curr_successors.size(); ++i) {
          pfel("\nsuccessor " << i << ":" << curr_successors[i]);
          if (visited[curr_successors[i]]) {
            pfel("visited successor" << visited[curr_successors[i]] << "|");
            continue;
          }
          // udpate the successors map
    // std::map<std::string, std::map<std::string, std::string>> valueTuple;

          // map<string, string> newSuccessorMap = meet(
          //     valueTuple[curr_successors[i]], 
          //     transferFunc(curr_successors[i], valueTuple[curInstructionStr], getInstructionFromString,
          //                getInstructionFromString[curr_successors[i]])
          //     );

          map<string, string> newSuccessorMap = meet(
              valueTuple[curr_successors[i]], valueTuple[curInstructionStr]);

          

          pf("OLD values of successor");
          printMapValues(valueTuple[curr_successors[i]]);

          pf("\nNEW values of successor");
          printMapValues(newSuccessorMap);

          valueTuple[curr_successors[i]] = newSuccessorMap;

          if (!inQ[curr_successors[i]] && Changed) {
            worklist.push(curr_successors[i]);
            inQ[curr_successors[i]] = true;
          }
        }

        // break;
        // get instruction from work list
        // mark it as visited
        // apply transfer function
        // All meet of the current map to all the successor nodes maps
        // unless already visited
        // check with the pervious values of the map and check if the current
        // ones are different if yes then set chagned to true
        // add the changes instruction in to the worklist
        /* code */
        pf("\nbelow is worklist\n\n");
        printQueue(worklist);
        pf("\n\n");
      }

    } while (Changed == true);

    std::error_code EC;
    string filename = __FILE__;
    size_t dotPosition = filename.find_last_of('.');

    // Remove the extension (including the dot)
    std::string filenameWithoutExtension = filename.substr(0, dotPosition);

    // Append the new extension
    std::string newFilename = filenameWithoutExtension + ".txt";

    // filename.at()
    raw_fd_ostream outFile("cool", EC);
    pf("outfile is"<<newFilename);
    // outFile << "THIS IS CRI"
    //         << "\n";

    // outFile<<F.getName().str()<<"\n";

    for (Function::iterator bb = F.begin(), e = F.end(); bb != e; bb++) {

      // std::string curr_block=bb->getName()  ;
      // std::string curr_block_name(bb->getName().data(),
      // bb->getName().size()); errs() << "BB name " << curr_block_name << "\n";

      // errs() << "BB size " << bb->size() << "\n";
      for (BasicBlock::iterator i = bb->begin(), i2 = bb->end(); i != i2; i++) {
        // pfel("\t\t\t||| " << *i << "\t\t\t |||");
        outFile << *i << " ";
        outs()<<*i << " ";
        // Instruction lineinstruction=dyn_cast<Instruction> (i);
        // linemap[linenumber++]=* i;
        // worklist.push(getStringFromInstruction(dyn_cast<Instruction>(i)));

        switch (i->getOpcode()) {
        default:
          break;

        case Instruction::Alloca: {
          // pf("\t\tAlloca Instructino\n");
          // pf(*i);

          std::string instructionSubstr =
              getresultOperand(dyn_cast<Instruction>(i));
          // pfel(" --> " << stripmod(instructionSubstr) << " = TOP");
          outFile << " --> " << addmod(instructionSubstr) << " = TOP";
          outFile << "\n";
          outs() << " --> " << addmod(instructionSubstr) << " = TOP";
          outs() << "\n";
          break;
        }
        case Instruction::Load: {
          pf("\t\tLoadddd Instruction\n");
          if (LoadInst *LI = dyn_cast<LoadInst>(i)) {
            std::string s;

            raw_string_ostream os(s);
            LI->printAsOperand(os, false);
            os.flush();
            std::string instructionSubstr = os.str();
            pf(*LI);
            std::string variableName = LI->getPointerOperand()->getName().str();
            unsigned numOperands = LI->getNumOperands();
            pfel("-->" << instructionSubstr << " = " << variableName);
            outFile << "-->" << addmod(instructionSubstr) << " = "
                    << valueTuple[getStringFromInstruction(
                           dyn_cast<Instruction>(i))][addmod(variableName)];
            outFile << "\t" << addmod(variableName) << " = "
                    << valueTuple[getStringFromInstruction(
                           dyn_cast<Instruction>(i))][addmod(variableName)];
            outs() << "-->" << addmod(instructionSubstr) << " = "
                    << valueTuple[getStringFromInstruction(
                           dyn_cast<Instruction>(i))][addmod(variableName)];
            outs() << "\t" << addmod(variableName) << " = "
                    << valueTuple[getStringFromInstruction(
                           dyn_cast<Instruction>(i))][addmod(variableName)];
            // std::string instructionSubstr =
            //         getresultOperand(dyn_cast<Instruction>(ist));
            //     // instructionSubstr is always stripped of % by default so
            //     add %
            //     // instructionSubstr = addmod(instructionSubstr);
            //     pfel(" --> " << instructionSubstr << " = TOP");
            //     newMap[instructionSubstr] = "TOP";
            // update from here
            // variable always has mode in load
            // instructionSubstr = addmod(instructionSubstr);

            // variableName=addmod(variableName);
            // string destinationOpernad = instructionSubstr;
            // if (isInteger(destinationOpernad)) {
            //   newMap[instructionSubstr] = destinationOpernad;

            // } else {
            //   pf("checkkkkkkkkk " << curMap[addmod(variableName)] << " "
            //                       << addmod(variableName));
            //   newMap[instructionSubstr] = curMap[addmod(variableName)];
            // }
            outFile << "\n";
            outs()<<"\n";
          }

          break;
        }
        case Instruction::Store: {
          pf("\t\tStore Instruction\n");
          if (StoreInst *Si = dyn_cast<StoreInst>(i)) {

            // errs() << " " << *ist << " ";
            pfel("STOREEE");

            unsigned numOperands = Si->getNumOperands();

            std::string variableName = Si->getPointerOperand()->getName().str();
            Value *storedValue = Si->getValueOperand();

            // pfel("1st operand/variable name :");
            pf("-->");
            variableName = addmod(variableName);

            pf(variableName << " = ");
            string destinationOpernad = getVariableNamefromoperand(storedValue);
            pfel(destinationOpernad);
            // val is int assign it to variable
            if (isInteger(destinationOpernad)) {
              // newMap[variableName] = destinationOpernad;
              outFile
                  << "-->" << variableName << " = "
                  << valueTuple[getStringFromInstruction(
                         dyn_cast<Instruction>(i))][addmod(destinationOpernad)];
              outs()
                  << "-->" << variableName << " = "<<destinationOpernad;
                  // << valueTuple[getStringFromInstruction(
                  //        dyn_cast<Instruction>(i))][addmod(destinationOpernad)];

            } else {
              outFile
                  << "-->" << variableName << " = "
                  << valueTuple[getStringFromInstruction(
                         dyn_cast<Instruction>(i))][addmod(destinationOpernad)];

              outFile
                  << "\t" << destinationOpernad << " = "
                  << valueTuple[getStringFromInstruction(
                         dyn_cast<Instruction>(i))][addmod(destinationOpernad)];
              // newMap[variableName] =
              //     meetValue(curMap[variableName], destinationOpernad);
              // newMap[variableName] =
              // curMap[getVariableNamefromoperand(storedValue)];
              outFile << "\n";
              // outs()<<"\n";
              outs()
                  << "-->" << variableName << " = "
                  << valueTuple[getStringFromInstruction(
                         dyn_cast<Instruction>(i))][addmod(destinationOpernad)];

              outs()
                  << "\t" << destinationOpernad << " = "
                  << valueTuple[getStringFromInstruction(
                         dyn_cast<Instruction>(i))][addmod(destinationOpernad)];
              // newMap[variableName] =
              //     meetValue(curMap[variableName], destinationOpernad);
              // newMap[variableName] =
              // curMap[getVariableNamefromoperand(storedValue)];
              
            }outs() << "\n";
          }
          break;
        }
        case Instruction::GetElementPtr: {
          pf("\t\t GET element\n");
          break;
        }

        case Instruction::Call: {
          if (CallInst *Si = dyn_cast<CallInst>(i)) {
            Function *CalledFunc = Si->getCalledFunction();
      if (CalledFunc) {
          string FuncName = CalledFunc->getName().str();
          if(FuncName=="printf"){
            outs() << "-->"<< getVariableNamefromoperand(Si->getOperand(1))<<"="<<valueTuple[getStringFromInstruction(
                         dyn_cast<Instruction>(i))][getVariableNamefromoperand(Si->getOperand(1))] ;
          
          }else
          {
            // outs()<<getVariableNamefromoperand(Si->getOperand(1))<<"IS BOTTTTTTTTT"
            // newMap[getVariableNamefromoperand(Si->getOperand(1))] = "BOTTOM";
            outs() << "-->" << getVariableNamefromoperand(Si->getOperand(1))
                    << "=BOTTOM";

          }

          // return FuncName.equals("printf") || FuncName.equals("scanf");
      }
            // iterateME(Si);

            // errs()<<"+++"<< getVariableNamefromoperand(Si->getOperand(0));
            // errs()<<"++"<<getVariableNamefromoperand(Si->getOperand(1));
            // iterateME(Si);
            // pf("operand value is\n");
            // pfop(Si->getOperand(1));pf("\n::::");
            // pf(getVariableNamefromoperand(Si->getOperand(1)));
            outFile << "-->" << getVariableNamefromoperand(Si->getOperand(1))
                    << "=BOTTOM";
            // outs() << "-->" << getVariableNamefromoperand(Si->getOperand(1))
            //         << "=BOTTOM";
            
                    // << "=BOTTOM";
            // newMap[getVariableNamefromoperand(Si->getOperand(1))] = "BOTTOM";
          }
          outFile << "\n";
          outs()<<"\n";
          break;
        }
        case Instruction::ICmp: {
          outFile << "\n";
          outs()<<"\n";
          break;
        }
        case Instruction::Ret:
        case Instruction::Br:
        case Instruction::Invoke: {
          break;
        }

        // Binary Instructions
        case Instruction::Add:
        case Instruction::Sub:
        case Instruction::Mul:
        case Instruction::SDiv:
        case Instruction::UDiv:
        case Instruction::SRem:
        case Instruction::URem:
        case Instruction::Shl:
        case Instruction::LShr:
        case Instruction::AShr:
        case Instruction::And:
        case Instruction::Or:
        case Instruction::Xor: {
          // outFile << getresultOperand(dyn_cast<Instruction>(i)) << "= ";
          // outFile << getVariableNamefromoperand(i->getOperand(0)) << "= ";
          // outFile << getVariableNamefromoperand(i->getOperand(1)) << "= ";

          string op1, op2;
          op1 = getVariableNamefromoperand(i->getOperand(0));
          op2 = getVariableNamefromoperand(i->getOperand(1));

          outFile << "-->" << getresultOperand(dyn_cast<Instruction>(i)) << "= "
                  << valueTuple[getStringFromInstruction(dyn_cast<Instruction>(
                         i))][getresultOperand(dyn_cast<Instruction>(i))]
                  << "\t";
          outs() << "-->" << getresultOperand(dyn_cast<Instruction>(i)) << "= "
                  << valueTuple[getStringFromInstruction(dyn_cast<Instruction>(
                         i))][getresultOperand(dyn_cast<Instruction>(i))]
                  << "\t";
          

          if (!isInteger(op1)) {
            op1 = valueTuple[getStringFromInstruction(dyn_cast<Instruction>(i))]
                            [op1];
            outFile << getVariableNamefromoperand(i->getOperand(0)) << " = "
                    << op1 << "\t";
            outs()<< getVariableNamefromoperand(i->getOperand(0)) << " = "
            << op1 << "\t";
          }
          if (!isInteger(op2)) {
            op2 = valueTuple[getStringFromInstruction(dyn_cast<Instruction>(i))]
                            [op2];
            outFile << getVariableNamefromoperand(i->getOperand(1)) << " = "
                    << op2;
            outs()<< getVariableNamefromoperand(i->getOperand(1)) << " = "
                    << op2;
          }

          pfel("\t\tBinary Instruction\n" << *i);
          // outs()<<"\t\tBinary Instruction\n" << *i<<"\n";
          // iterateME(dyn_cast<Instruction>(i));

          // pf("\t\ttesttt\t\t");
          // pfopel(i->getOperand(0));

          // pf(getVariableNamefromoperand(i->getOperand(0)) << "= ");
          // pf(getVariableNamefromoperand(i->getOperand(1)) << "= ");

          outFile << "\n";
          outs()<<"\n";
          break;
        }
        }

        // store i32 %5, i32* %6, align 4

        if (BranchInst *BI = dyn_cast<BranchInst>(i)) {
          // Do something with branch instruction BI
          // pfel(i->getOperand(0));
          // pfel("found you My Branch instruction \t\t");
          outFile << "\n";
          outs()<<"\n";
        }
      }
      // for (BasicBlock &BB : F) {
      //       for (Instruction &I : BB) {
      //         pfel(I);
      //           // if (StoreInst *SI = dyn_cast<StoreInst>(&I)) {
      //           //     // This instruction is a store instruction
      //           //     errs() << "Found store instruction: " << *SI <<
      // "\n";
      //           // }
      //       }
      //   }
    }
    outFile.close();
    // outfile.close();

    return false;
  }
}; // end of struct Hello
} // end of anonymous namespace

char constant_p::ID = 0;
static RegisterPass<constant_p> X("libCP_given", "Constant Propagation Pass for Assignment",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);

