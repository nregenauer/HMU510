#include "m_pd.h"
#include "math.h"
#include "string.h"
#define kPI 3.14159265358979323846
#define radians 2 * 3.14159265358979323846

static t_class *optimizer_class;

//function to return factorial for optimizer
int factorial (int n){
  int r = 1;
  for (int i=2; i<=n; i++)
    r *= i;
  return r;
}

//optimizer: returns coefficients for optimization
typedef struct _optimizer {
  t_object x_obj;
  t_outlet* out;
  t_float x_f;

}t_optimizer;

//method to create in phase coefficients
//and pass them out as a list
void optimizer_bang(t_optimizer *x){
  float f1 = x->x_f;
  float coeff = factorial(f1)*factorial(f1);
  //just creating a list of arbitrary length
  float list [30];
  t_atom *list_out;
  //creating number of inphase coefficients
  //the user needs to know signs/what inlets to apply these to
  for(int i=0; i < (int)f1; i++){
    list[i] = coeff / ((float) factorial(f1-i) *  (float) factorial(f1+i));
  }
  list_out  = (t_atom*)t_getbytes((f1)*sizeof(t_atom));
  //casting each element in list to float type
  for(int i=0;i<f1;i++){
    SETFLOAT(list_out+i,list[i]);
  }
  //passing out list
  outlet_list(x->out, 0, f1, list_out);
  //freeing memory
  t_freebytes(list_out,30*sizeof(t_atom));
}

void *optimizer_new(t_symbol *s, int argc, t_atom * argv){
  t_optimizer *x = (t_optimizer *)pd_new(optimizer_class);
  post("creating a new optimizer");
  if(argc==0){
    x->x_f = 1; //creating default argument
  }
  else{
  x->x_f = atom_getfloat(argv);}
  x->out = outlet_new(&x->x_obj, gensym("list"));
return(void *)x;
}

void optimizer_setup(void){
  optimizer_class = class_new(gensym("optimizer"), (t_newmethod)optimizer_new,
                            0, sizeof(t_optimizer),
                            CLASS_DEFAULT,
                            A_GIMME,
                            0);
class_addbang(optimizer_class, optimizer_bang);
post("Simple optimizer external: generates externals based on order of system");
}


static t_class *encoder1_class;
typedef struct _encoder1 {
  t_object x_obj;
  t_sample x_f;
  t_inlet *x_in2;
  t_inlet *x_in3;
  int order;

}t_encoder1;

t_int *encoder1_perform(t_int *w)
{
    //this routine was provided on canvas!
    t_encoder1 *x = (t_encoder1 *)(w[1]);
    //first sample -> sound getting panned
    t_sample  *APin1 =    (t_sample *)(w[2]);
    //second and third samples -> panning frequency
    t_sample  *APin2 =    (t_sample *)(w[3]);
    t_sample  *APin3 =    (t_sample *)(w[4]);
    //output to speakers
    t_sample  *APout1 =    (t_sample *)(w[5]);
    t_sample  *APout2 =    (t_sample *)(w[6]);
    t_sample  *APout3 =    (t_sample *)(w[7]);
    t_sample  *APout4 =    (t_sample *)(w[8]);
    int APn = (int)(w[9]);
    //local variables for audio input
    t_sample sample1, sample2, sample3;
    while (APn--)
    {
        sample1 = (*APin1++); //audio input 1
        sample2 = (*APin2++) * radians; //audio input 2 for azimuth
        sample3 = (*APin3++ - 0.5) * kPI; //audio input 3 for elevation
        (*APout1++) =   sample1 * 0.707; //W
        (*APout2++) =   sample1 * cosf(sample2); //X
        (*APout3++) =   sample1 * sinf(sample2); //Y
        (*APout4++) =   sample1 * sinf(sample3); //Z
        //all of this math comes from the readings posted online
        //really simple first order conversions
        }
    return (w+10);
}

t_int* encoder2_perform(t_int *w){
  t_encoder1 *x = (t_encoder1 *)(w[1]);
  t_sample *APin1 = (t_sample *)(w[2]);
  t_sample *APin2 = (t_sample *)(w[3]);
  t_sample *APin3 = (t_sample *)(w[4]);
  t_sample  *APout1 =    (t_sample *)(w[5]);
  t_sample  *APout2 =    (t_sample *)(w[6]);
  t_sample  *APout3 =    (t_sample *)(w[7]);
  t_sample  *APout4 =    (t_sample *)(w[8]);
  t_sample  *APout5 =    (t_sample *)(w[9]);
  int APn = (int)(w[10]);

  t_sample sample1, sample2, sample3;
while(APn--){
  sample1 = (*APin1++); //audio input 1
  sample2 = (*APin2++) * radians; //audio input 2 for azimuth
  sample3 = (*APin3++ - 0.5) * kPI; //audio input 3 for elevatio

  (*APout1++) =   sample1 * 0.707; //W
  (*APout2++) =   sample2 * cosf(sample2); //X
  (*APout3++) =   sample1 * sinf(sample2); //Y
  (*APout4++) =   sample1 * cosf(2 * sample2); //U
  (*APout5++) =   sample1 * sinf(2 * sample2); //V
}
return (w+11);
}

t_int *encoder3_perform(t_int *w){
  t_encoder1 *x =  (t_encoder1 *)(w[1]);
  t_sample *APin1 = (t_sample *)(w[2]);
  t_sample *APin2 = (t_sample *)(w[3]);
  t_sample *APin3 = (t_sample *)(w[4]);
  t_sample  *APout1 =    (t_sample *)(w[5]);
  t_sample  *APout2 =    (t_sample *)(w[6]);
  t_sample  *APout3 =    (t_sample *)(w[7]);
  t_sample  *APout4 =    (t_sample *)(w[8]);
  t_sample  *APout5 =    (t_sample *)(w[9]);
  t_sample *APout6 = (t_sample *)(w[10]);
  t_sample *APout7 = (t_sample *)(w[11]);
  int APn = (int)(w[12]);

t_sample sample1, sample2, sample3;
while(APn--){
  sample1 = (*APin1++); //audio input 1
  sample2 = (*APin2++) * radians; //audio input 2 for azimuth
  sample3 = (*APin3++ - 0.5) * kPI; //audio input 3 for elevatio

  (*APout1++) =   sample1 * 0.707; //W
  (*APout2++) =   sample1 * cosf(sample2); //X
  (*APout3++) =   sample1 * sinf(sample2); //Y
  (*APout4++) =   sample1 * cosf(2 * sample2); //U
  (*APout5++) =   sample1 * sinf(2 * sample2); //V
  (*APout6++) =   sample1 * cosf(3 * sample2); //P
  (*APout7++) =   sample1 * sinf(3 * sample2); //Q
}
return (w+13);
}

void encoder1_dsp(t_encoder1 *x, t_signal **sp){
  if(x->order == 1){
      dsp_add(encoder1_perform, 9, x ,sp[0]->s_vec,sp[1]->s_vec,sp[2]->s_vec,sp[3]->s_vec,sp[4]->s_vec,sp[5]->s_vec,sp[6]->s_vec,sp[0]->s_n);
  }
  if(x->order == 2){
      dsp_add(encoder2_perform, 10, x ,sp[0]->s_vec,sp[1]->s_vec,sp[2]->s_vec,sp[3]->s_vec,sp[4]->s_vec,sp[5]->s_vec,sp[6]->s_vec,sp[7]->s_vec,sp[0]->s_n);}
  if(x->order == 3){
    dsp_add(encoder3_perform, 12, x ,sp[0]->s_vec,sp[1]->s_vec,sp[2]->s_vec,sp[3]->s_vec,sp[4]->s_vec,sp[5]->s_vec,sp[6]->s_vec,sp[7]->s_vec,sp[8]->s_vec,sp[9]->s_vec,sp[0]->s_n);
  }

}

void *encoder1_new (t_floatarg o){
  t_encoder1 *x = (t_encoder1 *)pd_new(encoder1_class);
  x->order = o;
  inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal,&s_signal);
  inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal,&s_signal);

  switch(x->order){
    case 1:
      outlet_new(&x->x_obj, gensym ("signal"));
      outlet_new(&x->x_obj, gensym ("signal"));
      outlet_new(&x->x_obj, gensym ("signal"));
      outlet_new(&x->x_obj, gensym ("signal"));
    break;
    case 2:
      outlet_new(&x->x_obj, gensym ("signal"));
      outlet_new(&x->x_obj, gensym ("signal"));
      outlet_new(&x->x_obj, gensym ("signal"));
      outlet_new(&x->x_obj, gensym ("signal"));
      outlet_new(&x->x_obj, gensym ("signal"));
    break;
    case 3:
      outlet_new(&x->x_obj, gensym ("signal"));
      outlet_new(&x->x_obj, gensym ("signal"));
      outlet_new(&x->x_obj, gensym ("signal"));
      outlet_new(&x->x_obj, gensym ("signal"));
      outlet_new(&x->x_obj, gensym ("signal"));
      outlet_new(&x->x_obj, gensym ("signal"));
      outlet_new(&x->x_obj, gensym ("signal"));
    break;

  }

  return(void *)x;
}

void encoder1_setup(void){
  encoder1_class = class_new(gensym("encoder1"),(t_newmethod)encoder1_new,0,sizeof(t_encoder1),CLASS_DEFAULT,A_DEFFLOAT,0);
  class_addmethod(encoder1_class,(t_method)encoder1_dsp,gensym("dsp"),0);

  CLASS_MAINSIGNALIN(encoder1_class, t_encoder1, x_f);
  post("generic encoder!");
}

static t_class *decoder1_class;

typedef struct _decoder1 {
  t_object x_obj;
  t_sample x_f;
  t_float order;
  float dectype;

}t_decoder1;

t_int *decoder1_perform_mono(t_int *w){
  //just creating a first order decoder1
  //mono decoder can be used with any number of input signals
  //you just lose a LOT of data if you use it with higher orders
  t_decoder1 *x = (t_decoder1 *)(w[1]);
  t_sample *APin1 = (t_sample *)(w[2]);
  t_sample *APin2 = (t_sample *)(w[3]);
  t_sample *APin3 = (t_sample *)(w[4]);
  t_sample *APin4 = (t_sample *)(w[5]);
  t_sample *APout1 = (t_sample *)(w[6]);
  int APn = (int)(w[7]);
  //local variables for the ambisonic channels
  t_sample sample1, sample2, sample3, sample4;

  while (APn--)
  {
  //B-Format Input
  sample1 = (*APin1++); // W
  //this is multiplied by 1->0th order coefficient

  //Basic mono decoder
  //This basically just lets us play back an ambisonic soundfield
  //on a single speaker
  //it only takes data from one input signal ?
  //also, multiplying by speaker coefficient
  //found on canvas
  (*APout1++) = sample1 * 0.707;

  }
  return (w+8);
}

t_int *decoder1_stereo(t_int *w){
  //just creating a first order decoder1
  //going to try and call different decoders
  //depending on the creation arguments

  t_decoder1 *x = (t_decoder1 *)(w[1]);
  t_sample *APin1 = (t_sample *)(w[2]);
  t_sample *APin2 = (t_sample *)(w[3]);
  t_sample *APin3 = (t_sample *)(w[4]);
  t_sample *APin4 = (t_sample *)(w[5]);

  t_sample *APout1 = (t_sample *)(w[6]);
  t_sample *APout2 = (t_sample *)(w[7]);
  int APn = (int)(w[8]);
  //local variables for the ambisonic channels
  t_sample sample1, sample2, sample3, sample4,sample5;

  while (APn--)
  {
  //B-Format Input
  sample1 = (*APin1++); //again, treating as first order
  sample2 = (*APin2++); //leave coefficients as 1 or apply externally
  sample3 = (*APin3++);
  sample4 = (*APin4++);
  //Basic Stereo Decoder
  //This basically just lets us play back an ambisonic soundfield
  //on a limited range of speakers
  //multiplying by speaker coefficients found on canvas
  (*APout1++) = sample1 * 0.707 + sample3 * 0.5;
  (*APout2++) = sample1 * 0.707 + sample3 * -0.5;

  }
  return (w+9);
}

t_int *decoder1_quad(t_int *w){
  t_decoder1 *x = (t_decoder1 *)(w[1]);
  t_sample  *APin1 =    (t_sample *)(w[2]);
  t_sample  *APin2 =    (t_sample *)(w[3]);
  t_sample  *APin3 =    (t_sample *)(w[4]);
  t_sample  *APin4 =    (t_sample *)(w[5]);
  t_sample  *APout1 =    (t_sample *)(w[6]);
  t_sample  *APout2 =    (t_sample *)(w[7]);
  t_sample  *APout3 =    (t_sample *)(w[8]);
  t_sample  *APout4 =    (t_sample *)(w[9]);
  int          APn =           (int)(w[10]);

  t_sample sample1, sample2, sample3, sample4;

  while (APn--)
  {

      sample1 = (*APin1++);
      sample2 = (*APin2++);
      sample3 = (*APin3++);
      sample4 = (*APin4++);


      //multiplying signals by speaker coefficients
      //found on canvas
      (*APout1++) =   sample1 * 0.3536 + sample2 * 0.1768 + sample3 * 0.1768;
      (*APout2++) =   sample1 * 0.3536 + sample2 * -0.1768 + sample3 * 0.1768;
      (*APout3++) =   sample1 * 0.3536 + sample2 * -0.1768 + sample3 * -0.1768;
      (*APout4++) =   sample1 * 0.3536 + sample2 * 0.1768 + sample3 * -0.1768;
}
return (w+11);
}

t_int *decoder1_hex(t_int *w)
{
    t_decoder1 *x = (t_decoder1 *)(w[1]);
    t_sample  *APin1 =    (t_sample *)(w[2]);
    t_sample  *APin2 =    (t_sample *)(w[3]);
    t_sample  *APin3 =    (t_sample *)(w[4]);
    t_sample  *APin4 =    (t_sample *)(w[5]);
    t_sample  *APin5 =    (t_sample *)(w[6]);
    t_sample  *APout1 =    (t_sample *)(w[7]);
    t_sample  *APout2 =    (t_sample *)(w[8]);
    t_sample  *APout3 =    (t_sample *)(w[9]);
    t_sample  *APout4 =    (t_sample *)(w[10]);
    t_sample  *APout5 =    (t_sample *)(w[11]);
    t_sample  *APout6 =    (t_sample *)(w[12]);
    int          APn =           (int)(w[13]);

    t_sample sample1, sample2, sample3, sample4, sample5;

    while (APn--)
    {

        //because of the way that I set mine up, I really don't want to
        //apply coefficients in here-> drains a lot of the flexibility of the external
        //if you're forced to apply the same coefficients
        sample1 = (*APin1++);
        sample2 = (*APin2++);
        sample3 = (*APin3++);
        sample4 = (*APin4++);
        sample5 = (*APin5++);

        //hex decoder
        //multiplying the signals by coefficients from canvas
        (*APout1++) =   sample1 * 0.2357 + sample2 *  0.1987 + sample3 *  0.1147 + sample4 *  0.0321 * sample5 * 0.0556;
        (*APout2++) =   sample1 * 0.2357 + sample2 *  0.0000 + sample3 *  0.2294 + sample4 *  -0.0643 * sample5 * 0.0000;
        (*APout3++) =   sample1 * 0.2357 + sample2 *  -0.1987 + sample3 *  0.1147 + sample4 *  0.0321 * sample5 * -0.0556;
        (*APout4++) =   sample1 * 0.2357 + sample2 *  -0.1987 + sample3 *  -0.1147 + sample4 *  0.0321 * sample5 * 0.0556;
        (*APout5++) =   sample1 * 0.2357 + sample2 *  0.0000 + sample3 *  -0.2294 + sample4 * -0.0643 * sample5 * 0.0000;
        (*APout6++) =   sample1 * 0.2357 + sample2 *  0.1987 + sample3 *  -0.1147 + sample4 * 0.0321 * sample5 * -0.0556;
    }


    return (w+14);

}

void decoder1_dsp(t_decoder1 *x, t_signal **sp){
  //basically just choosing our perform routine
  //based on number of speakers we are decoding for!
  if(x->dectype==0){
  dsp_add(decoder1_perform_mono, 7, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec, sp[4]->s_vec, sp[0]->s_n);
  }
  if(x->dectype==1){
    dsp_add(decoder1_stereo, 8, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec, sp[4]->s_vec, sp[5]->s_vec, sp[0]->s_n);
  }
  if(x->dectype==2){
    dsp_add(decoder1_quad, 10, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec, sp[4]->s_vec, sp[5]->s_vec, sp[6]->s_vec, sp[7]->s_vec, sp[0]->s_n);
  }
  if(x->dectype==3){
    dsp_add(decoder1_hex, 13, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec, sp[4]->s_vec, sp[5]->s_vec, sp[6]->s_vec, sp[7]->s_vec, sp[8]->s_vec, sp[9]->s_vec, sp[10]->s_vec, sp[0]->s_n);
  }
}

void *decoder1_new(t_symbol *s, int argc, t_atom *argv){
  t_decoder1 *x = (t_decoder1 *)pd_new(decoder1_class);
  char str[1000];
  unsigned int length_str = 1000;
  //if we have no arguments, pick a default order and speaker config
  if(argc == 0){
    x->order = 1;
    x->dectype = 1;
    outlet_new(&x->x_obj, gensym ("signal"));

  }
  //if we only have an order passed in,
  //pick out stereo config to decoder for
  else if(argc==1){
    x->order = atom_getfloat(argv);
    x->dectype = 1;
    outlet_new(&x->x_obj, gensym ("signal"));
    //default is stereo so need to make two outlets
  }
  else if(argc==2){
    //i just allocate extra outlets where I evaluate the string input
    //saves me from having to create another switch/case
    x->order = atom_getfloat(argv);
    atom_string(argv+1,str,length_str);
    if(strcmp(str,"mono")==0){
      x->dectype = 0;
    }
    else if(strcmp(str,"stereo")==0){
      x->dectype=1;
      outlet_new(&x->x_obj, gensym ("signal"));

    }
    else if(strcmp(str,"quad")==0){
      x->dectype=2;
      outlet_new(&x->x_obj, gensym ("signal"));
      outlet_new(&x->x_obj, gensym ("signal"));
      outlet_new(&x->x_obj, gensym ("signal"));

    }
    else if(strcmp(str,"hex")==0){
      x->dectype=3;
      outlet_new(&x->x_obj, gensym ("signal"));
      outlet_new(&x->x_obj, gensym ("signal"));
      outlet_new(&x->x_obj, gensym ("signal"));
      outlet_new(&x->x_obj, gensym ("signal"));
      outlet_new(&x->x_obj, gensym ("signal"));

    }
    else{
      x->dectype=1;
      outlet_new(&x->x_obj, gensym ("signal"));
    }
  }
  else{
    x->order=1;
    x->dectype =1;
    outlet_new(&x->x_obj, gensym ("signal"));
  }

  if(x->order == 2){
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, gensym("signal"), gensym("signal"));
  }
  else if(x->order==3){
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, gensym("signal"), gensym("signal"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, gensym("signal"), gensym("signal"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, gensym("signal"), gensym("signal"));
  }
  //we always want to make at least 3 inlets and 1 outlet
  inlet_new(&x->x_obj, &x->x_obj.ob_pd, gensym("signal"), gensym("signal"));
  inlet_new(&x->x_obj, &x->x_obj.ob_pd, gensym("signal"), gensym("signal"));
  inlet_new(&x->x_obj, &x->x_obj.ob_pd, gensym("signal"), gensym("signal"));
  outlet_new(&x->x_obj, gensym ("signal"));

  return (void*)x;
}
void decoder1_setup(void){
  decoder1_class = class_new(gensym("decoder1"),
        (t_newmethod)decoder1_new,
        0, sizeof(t_decoder1),
        CLASS_DEFAULT, A_GIMME, A_GIMME, 0);

class_addmethod(decoder1_class,
              (t_method)decoder1_dsp, gensym("dsp"), 0);
//this helps us accept signals on 1st inlet
CLASS_MAINSIGNALIN(decoder1_class, t_decoder1, x_f);
post("Created decoder1");

}

void submission_setup(void){
  decoder1_setup();
  encoder1_setup();
  post("Created basic encoder and decoder");
}
