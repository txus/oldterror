#include <stdlib.h>
#include <terror/machine.h>
#include <terror/registers.h>
#include <terror/primitives.h>
#include <terror/vmmethod.h>
#include <terror/runtime.h>
#include <terror/dbg.h>
#include <terror/util.h>

Object *TrueObject;
Object *FalseObject;
Object *NilObject;
Object *Lobby;

Machine*
Machine_new(Instruction **instructions, int instructions_count, long *literals, int literals_count, Object **locals, int locals_count, int registers_count)
{
  Machine *machine = calloc(1, sizeof(Machine));
  machine->instructions       = instructions;
  machine->instructions_count = instructions_count;
  machine->literals           = literals;
  machine->literals_count     = literals_count;
  machine->locals             = locals;
  machine->locals_count       = locals_count;
  machine->registers_count    = registers_count;
  return machine;
}

Object*
Machine_run(Machine *machine, Object *self)
{
  int executed = 0;
  Instruction **ip = machine->instructions;
  long *literals  = machine->literals;
  Object **locals = machine->locals;

  Object **regs = calloc(machine->registers_count, sizeof(Object*));

  while(1) {
    Instruction *i = *ip;
    switch(i->opcode) {
      case NOOP:
        debug("NOOP");
        break;
      case MOVE: {
        debug("MOVE %i %i", i->fields.a, i->fields.b);
        REGISTER(regs[i->fields.a], regs[i->fields.b]);
        break;
      }
      case LOADI: {
        debug("LOADI %i %i", i->fields.a, i->fields.b);
        CLEAN_REGISTER(regs[i->fields.a]);
        REGISTER(regs[i->fields.a], Integer_new((int)literals[i->fields.b]));
        break;
      }
      case LOADS: {
        debug("LOADS %i %i", i->fields.a, i->fields.b);
        CLEAN_REGISTER(regs[i->fields.a]);
        bstring literal = bfromcstr((char*)literals[i->fields.b]);
        REGISTER(regs[i->fields.a], String_new(literal));
        break;
      }
      case LOADNIL: {
        debug("LOADNIL %i", i->fields.a);
        CLEAN_REGISTER(regs[i->fields.a]);
        regs[i->fields.a] = NilObject;
        break;
      }
      case LOADBOOL: {
        debug("LOADBOOL %i %i", i->fields.a, i->fields.b);
        CLEAN_REGISTER(regs[i->fields.a]);
        if (i->fields.b)
        {
          regs[i->fields.a] = TrueObject;
        } else {
          regs[i->fields.a] = FalseObject;
        }
        break;
      }
      case LOADSELF: {
        debug("LOADSELF %i", i->fields.a);
        CLEAN_REGISTER(regs[i->fields.a]);
        REGISTER(regs[i->fields.a], self);
        break;
      }
      case JMP: {
        debug("JMP %i", i->fields.a);
        int j=1;
        for(j=1; j < i->fields.a; j++) {
          ip++;
        }
        break;
      }
      case JIF: {
        debug("JIF %i %i", i->fields.a, i->fields.b);

        Object *value = regs[i->fields.b];
        if (value == FalseObject || value == NilObject) {
          int j=1;
          for(j=1; j < i->fields.a; j++) {
            ip++;
          }
        }

        break;
      }
      case JIT: {
        debug("JIT %i %i", i->fields.a, i->fields.b);

        Object *value = regs[i->fields.b];
        if (value != FalseObject && value != NilObject) {
          int j=1;
          for(j=1; j < i->fields.a; j++) {
            ip++;
          }
        }

        break;
      }
      case LOADLOCAL: {
        debug("LOADLOCAL %i %i", i->fields.a, i->fields.b);
        CLEAN_REGISTER(regs[i->fields.a]);
        REGISTER(regs[i->fields.a], locals[i->fields.b]);
        break;
      }
      case SETLOCAL: {
        debug("SETLOCAL %i %i", i->fields.a, i->fields.b);
        locals[i->fields.b] = regs[i->fields.a];
        break;
      }
      case LOADSLOT: {
        debug("LOADSLOT %i %i %i", i->fields.a, i->fields.b, i->fields.c);
        Object *receiver  = regs[i->fields.b];
        bstring slot_name = bstrcpy(regs[i->fields.c]->value.string);

        Object *value = Object_get_slot(receiver, slot_name);

        CLEAN_REGISTER(regs[i->fields.a]);
        REGISTER(regs[i->fields.a], value);
        break;
      }
      case SETSLOT: {
        debug("SETSLOT %i %i %i", i->fields.a, i->fields.b, i->fields.c);
        Object *receiver  = regs[i->fields.a];
        bstring slot_name = bstrcpy(regs[i->fields.b]->value.string);
        Object *value     = retain(regs[i->fields.c]);

        Object_register_slot(receiver, slot_name, value);
        break;
      }
      case MAKEARRAY: {
        debug("MAKEARRAY %i %i %i", i->fields.a, i->fields.b, i->fields.c);
        CLEAN_REGISTER(regs[i->fields.a]);

        // Advance to the starting register
        Object **contents = regs;
        int j=0;
        for(j=0; j < i->fields.b; j++) contents++;

        Object *array = Array_new(contents, i->fields.c);

        REGISTER(regs[i->fields.a], array);
        break;
      }
      case SEND: {
        debug("SEND %i %i %i", i->fields.a, i->fields.b, i->fields.c);

        Object  *receiver  = regs[i->fields.a];
        bstring message    = regs[i->fields.b]->value.string;
        int arity          = Object_lookup_method_arity(receiver, message);

        Object **argv = extract_args(regs, i->fields.c, arity);

        // Special case: Function#apply
        bstring special_call = bfromcstr("apply");
        if(receiver->type == tFunction && bstrcmp(message, special_call)==0) {
          Object *slf = argv[0];
          // shift arguments to the right, because the first argument will be
          // self.
          argv++;

          REGISTER(regs[i->fields.a],
            Function_call(receiver, slf, argv, arity, machine->registers_count));

          /* free(argv); */
          release(receiver);
          break; // end instruction
        }
        bdestroy(special_call);
        // End of special case

        if(arity > -2) {
          REGISTER(regs[i->fields.a],
              call_method(receiver, message, argv, arity, machine->registers_count));
        } else {
          // Look up slot
          REGISTER(regs[i->fields.a],
              Object_lookup_slot(receiver, message));
        }

        free(argv);
        release(receiver);

        break;
      }
      case RET: {
        debug("RET %i", i->fields.a);
        Registers_cleanup(regs, machine->registers_count, i->fields.a);
        Object *returnable = regs[i->fields.a];
        free(regs);
        return returnable;
        break;
      }
      case DUMP: {
        debug("DUMP");
        Registers_print(regs, machine->registers_count);
        break;
      }
    }
    executed++;
    assert(executed < machine->instructions_count && "Broken bytecode: executed too many instructions");
    ip++;
  }

  return self;
}
