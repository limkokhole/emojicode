//
//  ASTControlFlow_CG.cpp
//  Emojicode
//
//  Created by Theo Weidmann on 03/09/2017.
//  Copyright © 2017 Theo Weidmann. All rights reserved.
//

#include "ASTControlFlow.hpp"
#include "../Generation/CallCodeGenerator.hpp"
#include "../Generation/FunctionCodeGenerator.hpp"
#include "ASTProxyExpr.hpp"

namespace EmojicodeCompiler {

void ASTIf::generate(FunctionCodeGenerator *fg) const {
    auto *function = fg->builder().GetInsertBlock()->getParent();

    auto afterIfBlock = llvm::BasicBlock::Create(fg->generator()->context(), "afterIf");
    for (size_t i = 0; i < conditions_.size(); i++) {
        auto thenBlock = llvm::BasicBlock::Create(fg->generator()->context(), "then", function);
        auto elseBlock = llvm::BasicBlock::Create(fg->generator()->context(), "else", function);

        fg->builder().CreateCondBr(conditions_[i]->generate(fg), thenBlock, elseBlock);
        fg->builder().SetInsertPoint(thenBlock);
        blocks_[i].generate(fg);

        if (!certainlyReturned_) {
            fg->builder().CreateBr(afterIfBlock);
        }
        fg->builder().SetInsertPoint(elseBlock);
    }

    if (hasElse()) {
        blocks_.back().generate(fg);
    }

    if (!certainlyReturned_) {
        fg->builder().CreateBr(afterIfBlock);
        function->getBasicBlockList().push_back(afterIfBlock);
        fg->builder().SetInsertPoint(afterIfBlock);
    }
}

void ASTRepeatWhile::generate(FunctionCodeGenerator *fg) const {
    auto *function = fg->builder().GetInsertBlock()->getParent();

    auto afterBlock = llvm::BasicBlock::Create(fg->generator()->context(), "afterRepeatWhile");
    auto whileCondBlock = llvm::BasicBlock::Create(fg->generator()->context(), "whileCond", function);
    auto repeatBlock = llvm::BasicBlock::Create(fg->generator()->context(), "repeat", function);

    fg->builder().CreateBr(whileCondBlock);

    fg->builder().SetInsertPoint(whileCondBlock);
    fg->builder().CreateCondBr(condition_->generate(fg), repeatBlock, afterBlock);

    fg->builder().SetInsertPoint(repeatBlock);
    block_.generate(fg);
    fg->builder().CreateBr(whileCondBlock);

    function->getBasicBlockList().push_back(afterBlock);
    fg->builder().SetInsertPoint(afterBlock);
}

void ASTErrorHandler::generate(FunctionCodeGenerator *fg) const {
    // TODO: implement
//    value_->generate(fg);
//    fg->scoper().pushScope();
//    auto &var = fg->scoper().declareVariable(varId_, value_->expressionType());
//    fg->copyToVariable(var.stackIndex, false, value_->expressionType());
//    fg->pushVariableReference(var.stackIndex, false);
//    fg->wr().writeInstruction(INS_IS_ERROR);
//    fg->wr().writeInstruction(INS_JUMP_FORWARD_IF);
//    auto valueBlockCount = fg->wr().writeInstructionsCountPlaceholderCoin();
//    if (!valueIsBoxed_) {
//        var.stackIndex.increment();
//    }
//    var.type = valueType_;
//    valueBlock_.generate(fg);
//    fg->wr().writeInstruction(INS_JUMP_FORWARD);
//    auto errorBlockCount = fg->wr().writeInstructionsCountPlaceholderCoin();
//    valueBlockCount.write();
//    if (valueIsBoxed_) {
//        var.stackIndex.increment();
//    }
//    var.type = value_->expressionType().genericArguments()[0];
//    errorBlock_.generate(fg);
//    errorBlockCount.write();
//    fg->scoper().popScope(fg->wr().count());
}

void ASTForIn::generate(FunctionCodeGenerator *fg) const {
    // TODO: implement
//    fg->scoper().pushScope();
//
//    auto callCG = CallCodeGenerator(fg, INS_DISPATCH_PROTOCOL);
//
//    callCG.generate(*iteratee_, iteratee_->expressionType(), ASTArguments(position()), std::u32string(1, E_DANGO));
//
//    auto &itVar = fg->scoper().declareVariable(iteratorVar_, Type(PR_ENUMERATOR, false));
//    auto &elementVar = fg->scoper().declareVariable(elementVar_, elementType_);
//
//    fg->copyToVariable(itVar.stackIndex, false, Type(PR_ENUMERATEABLE, false));
//
//    fg->wr().writeInstruction(INS_JUMP_FORWARD);
//    auto placeholder = fg->wr().writeInstructionsCountPlaceholderCoin();
//
//    auto getVar = ASTProxyExpr(position(), itVar.type, [&itVar](auto *fg) {
//        fg->pushVariableReference(itVar.stackIndex, false);
//    });
//
//    auto delta = fg->wr().count();
//    callCG.generate(getVar, itVar.type, ASTArguments(position()), std::u32string(1, 0x1F53D));
//    fg->copyToVariable(elementVar.stackIndex, false, Type(PR_ENUMERATEABLE, false));
//    block_.generate(fg);
//    placeholder.write();
//
//    callCG.generate(getVar, itVar.type, ASTArguments(position()), std::u32string(1, E_RED_QUESTION_MARK));
//    fg->wr().writeInstruction(INS_JUMP_BACKWARD_IF);
//    fg->wr().writeInstruction(fg->wr().count() - delta + 1);
//    fg->scoper().popScope(fg->wr().count());
}

}  // namespace EmojicodeCompiler
