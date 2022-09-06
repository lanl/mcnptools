#ifndef __CYCLOPS_CLPARSER_HPP__
#define __CYCLOPS_CLPARSER_HPP__

#include <memory>
#include <set>
#include <map>
#include <iostream>
#include <cstdlib>

#include "CLMessageOp.hpp"
#include "CLSwitchOp.hpp"
#include "CLValueOp.hpp"
#include "CLMultiOp.hpp"
#include "CLPosValueOp.hpp"
#include "CLPosMultiOp.hpp"

#include "CYCLOpsExceptions.hpp"

#include "CLBasicHelpFormatter.hpp"

namespace cyclops {

template<typename H=CLBasicHelpFormatter<80> >
class CLParser {
public:
  CLParser(const std::string prog="") : m_prog(prog) {};

  // add different types of options
  CLMessageOp* AddOption(CLMessageOp& op) {
    CheckName(op.Name());
    CLMessageOp* retval = &op;
    m_messages[op.Name()] = retval;
    m_flg_order.push_back(op.Name());
    return retval;
  };

  CLSwitchOpBase* AddOption(CLSwitchOpBase& op) {
    CheckName(op.Name());
    CLSwitchOpBase* retval = &op;
    m_switchs[op.Name()] = retval;
    m_flg_order.push_back(op.Name());
    return retval;
  };

  CLValueOpBase* AddOption(CLValueOpBase& op) {
    CheckName(op.Name());
    CLValueOpBase* retval = &op;
    m_values[op.Name()] = retval;
    m_flg_order.push_back(op.Name());
    return retval;
  };

  CLMultiOpBase* AddOption(CLMultiOpBase& op) {
    CheckName(op.Name());
    CLMultiOpBase* retval = &op;
    m_multis[op.Name()] = retval;
    m_flg_order.push_back(op.Name());
    return retval;
  };

  CLPosValueOpBase* AddOption(CLPosValueOpBase& op) {
    CheckName(op.Name());
    CLPosValueOpBase* retval = &op;
    m_posvalues[op.Name()] = retval;
    m_pos_order.push_back(op.Name());
    return retval;
  };

  CLPosMultiOpBase* AddOption(CLPosMultiOpBase& op) {
    CheckName(op.Name());
    CLPosMultiOpBase* retval = &op;
    m_posmultis[op.Name()] = retval;
    m_pos_order.push_back(op.Name());
    return retval;
  };

  CLParser& MakeHelp() {
    CheckName("help");
    m_help_flag.reset( new CLTrueSwitchOp("--help,-h") );
    m_help_flag->Help("print usage infomation");
    m_switchs["help"] = m_help_flag.get();
    return *this;
  }

  std::string Prog() const {return m_prog;};

  std::string Description() const {return m_description;};
  CLParser& Description(const std::string& desc) {m_description = desc; return *this;};

  std::string Epilog() const {return m_epilog;};
  CLParser& Epilog(const std::string& epi) {m_epilog = epi; return *this;};

  size_t ParseOptions(const int argc, const char* const argv[]) {
    if( m_prog.size() == 0 ) m_prog = argv[0];

    std::string new_op_name, op_name;
    std::string flg;
    bool processing_flag = false;
    for(int i=1; i<argc; i++) {
      std::string arg(argv[i]);

      if( arg[0] == '-') { // assume new flag
        flg = arg;
        new_op_name = FlagToOp(arg);

        if( new_op_name.size() != 0 ) { // it is a flag
          op_name = new_op_name;

          if( m_flg_complete.find(op_name) != m_flg_complete.end() ) // already processed this flag
            throw CYCLOpsException("\'" + flg + "\' already processed");

          processing_flag = true;
          
          if( m_messages.find(op_name) != m_messages.end() ) { // process a flag if a message
            m_messages[op_name]->GetOstream() << m_messages[op_name]->GetMessage() << std::endl;
            if( m_messages[op_name]->GetExit() ) std::exit(0);
            processing_flag = false;
          }

          if( m_switchs.find(op_name) != m_switchs.end() ) { // process flag if a switch
            m_switchs[op_name]->Switch();
            m_flg_complete.insert(op_name);
            processing_flag = false;
          }

          continue;
        }
        // else process as argument
        
        // if( retval.size() == 0 )
        //   throw CYCLOpsException("Unrecognized flag \'" + flg + "\'");
      }

      // keep processing
      if( processing_flag ) { // processing a flag
        if(m_values.find(op_name) != m_values.end() ) {
          m_values[op_name]->Set(arg);
          m_flg_complete.insert(op_name);
          processing_flag = false;
        }
        else if(m_multis.find(op_name) != m_multis.end() ) {
          m_multis[op_name]->Add(arg);
          if( m_multis[op_name]->Count() == m_multis[op_name]->MaxArgs() ) {
            m_flg_complete.insert(op_name);
            processing_flag = false;
          }
        }
      }
      else { // must be positional
        // find the next incomplete positional argument
        for(auto it=m_pos_order.begin(); it!=m_pos_order.end(); ++it) {
          if( m_pos_complete.find(*it) == m_pos_complete.end() ) {
            op_name = *it;
            break;
          }
        }

        if(m_posvalues.find(op_name) != m_posvalues.end() ) {
          m_posvalues[op_name]->Set(arg);
          m_pos_complete.insert(op_name);
        }
        else if(m_posmultis.find(op_name) != m_posmultis.end() ) {
          m_posmultis[op_name]->Add(arg);
          if( m_posmultis[op_name]->Count() == m_posmultis[op_name]->MaxArgs() ) {
            m_pos_complete.insert(op_name);
          }
        }
        else { // don't know how to process the argument
          throw CYCLOpsException("error processing argument \'" + arg + "\'");
        }
      }
      
    }

    // process help if necessary
    if( m_help_flag ) {
      if( m_help_flag->Value() ) ProcessHelp();
    }

    return Validate();
  };

private:
  std::string m_prog;
  std::string m_description;
  std::string m_epilog;
  std::set<std::string> m_names;
  std::vector<std::string> m_flg_order;
  std::set<std::string> m_flg_complete;
  std::vector<std::string> m_pos_order;
  std::set<std::string> m_pos_complete;

  std::map<std::string,CLMessageOp*> m_messages;
  std::map<std::string,CLSwitchOpBase*> m_switchs;
  std::map<std::string,CLValueOpBase*> m_values;
  std::map<std::string,CLMultiOpBase*> m_multis;
  std::map<std::string,CLPosValueOpBase*> m_posvalues;
  std::map<std::string,CLPosMultiOpBase*> m_posmultis;

  std::shared_ptr<CLTrueSwitchOp> m_help_flag;

  void CheckName(const std::string& name) {
    if( m_names.find(name) != m_names.end() )
      throw CYCLOpsException("Option with name \'" + name + "\' already exists");
    m_names.insert(name);
  };

  std::string FlagToOp(const std::string& flg) {
    std::string retval;

    for(auto it=m_messages.begin(); it!=m_messages.end(); ++it) {
      if( flg==it->second->LongFlag() || flg==it->second->ShortFlag() ) {
        retval = it->first;
      }
    }

    for(auto it=m_switchs.begin(); it!=m_switchs.end(); ++it) {
      if( flg==it->second->LongFlag() || flg==it->second->ShortFlag() ) {
        retval = it->first;
      }
    }

    for(auto it=m_values.begin(); it!=m_values.end(); ++it) {
      if( flg==it->second->LongFlag() || flg==it->second->ShortFlag() ) {
        retval = it->first;
      }
    }

    for(auto it=m_multis.begin(); it!=m_multis.end(); ++it) {
      if( flg==it->second->LongFlag() || flg==it->second->ShortFlag() ) {
        retval = it->first;
      }
    }

    return retval;
  };

  size_t Validate() {
    size_t validated = 0;
    
    // iterate over flag values
    for(auto it=m_values.begin(); it!=m_values.end(); ++it) {
      if( it->second->Required() && m_flg_complete.find(it->first) == m_flg_complete.end() ) {
        throw CYCLOpsException("Option for flag \'" + it->second->LongFlag() + "\' required but not found");
      }
      validated += 1;
    }

    // iterate over flag multis
    for(auto it=m_multis.begin(); it!=m_multis.end(); ++it) {
      if( it->second->Required() && it->second->Count() < it->second->MinArgs() ) {
        throw CYCLOpsException("Option for flag \'" + it->second->LongFlag() + "\' required but not found/completed");
      }

      if( it->second->Count() < it->second->MinArgs() || it->second->Count() > it->second->MaxArgs() ) {
        throw CYCLOpsException("Invalid number of arguments for flag \'" + it->second->LongFlag() + "\'");
      }
      validated += it->second->Count();
    }

    // iterate over positional values
    for(auto it=m_posvalues.begin(); it!=m_posvalues.end(); ++it) {
      if( it->second->Required() && m_pos_complete.find(it->first) == m_pos_complete.end() ) {
        throw CYCLOpsException("Positional argument \'" + it->second->Name() + "\' required but not found");
      }
      validated += 1;
    }

    // iterate over positional multis
    for(auto it=m_posmultis.begin(); it!=m_posmultis.end(); ++it) {
      if( it->second->Required() && it->second->Count() < it->second->MinArgs() ) {
        throw CYCLOpsException("Positional argument \'" + it->second->Name() + "\' required but not found/completed");
      }

      if( it->second->Count() < it->second->MinArgs() || it->second->Count() > it->second->MaxArgs() ) {
        throw CYCLOpsException("Invalid number of arguments for \'" + it->second->Name() + "\'");
      }
      validated += it->second->Count();
    }

    return validated;
  };

  void ProcessHelp() {
    H formatter(m_prog,m_description,m_epilog);

    // process flag options in order
    for(auto order=m_flg_order.begin(); order!=m_flg_order.end(); ++order) {
      if(m_messages.find(*order) != m_messages.end()) {
        formatter.AddFlaggedOp(*m_messages[*order], 0);
      }

      if(m_switchs.find(*order) != m_switchs.end()) {
        formatter.AddFlaggedOp(*m_switchs[*order], 0);
      }

      if(m_values.find(*order) != m_values.end()) {
        formatter.AddFlaggedOp(*m_values[*order]);
      }

      if(m_multis.find(*order) != m_multis.end()) {
        formatter.AddFlaggedOp(*m_multis[*order], m_multis[*order]->MinArgs(), m_multis[*order]->MaxArgs());
      }
    }

    // process positional options in order
    for(auto order=m_pos_order.begin(); order!=m_pos_order.end(); ++order) {
      if(m_posvalues.find(*order) != m_posvalues.end()) {
        formatter.AddPositionalOp(*m_posvalues[*order]);
      }
      else if(m_posmultis.find(*order) != m_posmultis.end()) {
        formatter.AddPositionalOp(*m_posmultis[*order], m_posmultis[*order]->MinArgs(), m_posmultis[*order]->MaxArgs());
      }
    }

    formatter(std::cerr);
    std::exit(0);
  };
};

}

#endif
