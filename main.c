#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#define	M_LN2   0.6931471805599453094172321214581765680755001343602552541206
#define	M_LN3   1.0986122886681096913952452369225257046474905578227494517346 

//#define DEBUG_ASTABLE

GtkBuilder *builder;

char * frequ_template = " Hz";
char * capac_template = " F";
char * resis_template = " Ω";
char * time_template = " S";
char * SIprefs = "pnum kMGT";

struct SIPrefix_ret {
  long double value;
  char prefix;
} SIPrefix_ret ;

struct waveDrawParams {
  long double high ;
  long double low ;
}waveDrawParams_AStable,waveDrawParams_MONOStable;

void getSIPrefix(long double val){
  SIPrefix_ret.prefix =*(SIprefs+4);
 
  if(val<0.001){
    SIPrefix_ret.prefix =*(SIprefs+3);
    val*=1000;
  
    if(val<0.001){
      SIPrefix_ret.prefix =*(SIprefs+2);
      val*=1000;
    }
    if(val<0.001){
      SIPrefix_ret.prefix =*(SIprefs+1);
      val*=1000;
    }
    if(val<0.001){
      SIPrefix_ret.prefix =*(SIprefs);
      val*=1000;
    }
  }
  
  if(val>1000){
    SIPrefix_ret.prefix =*(SIprefs+5);
    val/=1000;
    
    if(val>1000){
      SIPrefix_ret.prefix =*(SIprefs+6);
      val/=1000;
    }
    if(val>1000){
      SIPrefix_ret.prefix =*(SIprefs+7);
      val/=1000;
    }
    if(val>1000){
      SIPrefix_ret.prefix =*(SIprefs+8);
      val/=1000;
    }
  }
  SIPrefix_ret.value = val;
}





gboolean Draw(GtkWidget *widget, cairo_t *cr, gpointer data,struct waveDrawParams * WDP){
  guint width, height;
  GdkRGBA color;
  cairo_text_extents_t te;
  double tmp_dbl;
  char * string;
  width  = gtk_widget_get_allocated_width (widget);
  height = gtk_widget_get_allocated_height (widget);
  printf("DRAW\n");
  cairo_rectangle(cr,0,0,width,height);
  gtk_style_context_get_color (gtk_widget_get_style_context (widget),0,&color);
  gdk_cairo_set_source_rgba (cr, &color);
  cairo_fill (cr);
  printf("%.15Lf:%.15Lf",WDP->high,WDP->low);
  if((WDP->high !=0)){
  cairo_set_source_rgb (cr, 0, 1, 0);
  cairo_move_to(cr,0,height*0.25);
  cairo_line_to (cr,width*(WDP->high/(WDP->high+WDP->low)),height*0.25);
  cairo_line_to (cr,width*(WDP->high/(WDP->high+WDP->low)),height*0.95);
  cairo_line_to (cr,width,height*0.95);
  cairo_set_line_width (cr, 2);
  cairo_stroke (cr);

  cairo_set_line_width (cr, 1);
  cairo_set_source_rgb (cr, 1, 1, 0);
  cairo_move_to(cr,0,height*0.1);
  cairo_line_to (cr,width,height*0.1);
  cairo_stroke (cr);
  cairo_move_to(cr,0,height*0.1);
  cairo_line_to (cr,width*0.05,height*0.09);
  cairo_stroke (cr);
  cairo_move_to(cr,0,height*0.1);
  cairo_line_to (cr,width*0.05,height*0.11);
  cairo_stroke (cr);
  cairo_move_to(cr,width,height*0.1);
  cairo_line_to (cr,width*0.95,height*0.09);
  cairo_stroke (cr);
  cairo_move_to(cr,width,height*0.1);
  cairo_line_to (cr,width*0.95,height*0.11);
  cairo_stroke (cr);


  cairo_set_source_rgb (cr, 1, 1, 0);
  cairo_move_to(cr,0,height*0.5);
  cairo_line_to (cr,width,height*0.5);

  cairo_stroke (cr);
  cairo_move_to(cr,0,height*0.5);
  cairo_line_to (cr,width*0.05,height*0.49);
  cairo_stroke (cr);
  cairo_move_to(cr,0,height*0.5);
  cairo_line_to (cr,width*0.05,height*0.51);
  cairo_stroke (cr);
  cairo_move_to(cr,width*(WDP->high/(WDP->high+WDP->low)),height*0.5);
  cairo_line_to (cr,width*(WDP->high/(WDP->high+WDP->low))-width*0.05,height*0.49);
  cairo_stroke (cr);
  cairo_move_to(cr,width*(WDP->high/(WDP->high+WDP->low)),height*0.5);
  cairo_line_to (cr,width*(WDP->high/(WDP->high+WDP->low))-width*0.05,height*0.51);
  cairo_stroke (cr);

  
  cairo_stroke (cr);
  cairo_move_to(cr,width*(WDP->high/(WDP->high+WDP->low)),height*0.5);
  cairo_line_to(cr,width*(WDP->high/(WDP->high+WDP->low))+width*0.05,height*0.49);
  cairo_stroke (cr);
  cairo_move_to(cr,width*(WDP->high/(WDP->high+WDP->low)),height*0.5);
  cairo_line_to (cr,width*(WDP->high/(WDP->high+WDP->low))+width*0.05,height*0.51);
  cairo_stroke (cr);
  
  cairo_move_to(cr,width,height*0.5);
  cairo_line_to (cr,width*0.95,height*0.49);
  cairo_stroke (cr);
  cairo_move_to(cr,width,height*0.5);
  cairo_line_to (cr,width*0.95,height*0.51);
  cairo_stroke (cr);
  
  string = (char*) calloc(16,sizeof(char));
  tmp_dbl = (WDP->high+WDP->low);
  getSIPrefix(tmp_dbl);
  sprintf(string,"%12.12Lf",SIPrefix_ret.value);
  *(string+13)=SIPrefix_ret.prefix;
  *(string+14)='S';
  *(string+15)=0;

  cairo_select_font_face (cr, "cairo:monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size (cr, width/13);
  cairo_text_extents (cr, string, &te);
  cairo_move_to (cr,width*0.5 - te.width / 2 - te.x_bearing,
    height*0.05 - te.height / 2 - te.y_bearing);
  cairo_show_text (cr, string);

  free(string);

  string = (char*) calloc(13,sizeof(char));
  tmp_dbl = WDP->high;
  getSIPrefix(tmp_dbl);
  sprintf(string,"%10Lf",SIPrefix_ret.value);
  *(string+10)=SIPrefix_ret.prefix;
  *(string+11)='S';
  *(string+12)=0;



  cairo_set_font_size (cr, width/16);
  cairo_text_extents (cr, string, &te);
  cairo_move_to (cr,width*(WDP->high/(WDP->high+WDP->low))*0.5 - te.width / 2 - te.x_bearing,
		 height*0.54 - te.height / 2 - te.y_bearing);
  cairo_show_text (cr, string);
  
   free(string);


   }


 return FALSE;

}

gboolean DrawMONOStable(GtkWidget *widget, cairo_t *cr, gpointer data){
   printf("DrawM\n");
  return Draw(widget,cr,data,&waveDrawParams_MONOStable);
}

gboolean DrawAStable(GtkWidget *widget, cairo_t *cr, gpointer data){
  // printf("DrawA\n");
  return Draw(widget,cr,data,&waveDrawParams_AStable);
}

static gboolean Monostable_inputChanged(GtkWidget *widget, GdkEvent *event, gpointer data){
  GtkEntry * entryR = GTK_ENTRY(gtk_builder_get_object(builder,"entryMonostableR")) ;
  GtkEntry * entryC  = GTK_ENTRY(gtk_builder_get_object(builder,"entryMonostableC")); 
  GtkComboBoxText * UnitC = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder,"comboboxtextMonostableUnitC"));
  GtkComboBoxText * UnitR = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder,"comboboxtextMonostableUnitR"));
  GtkLabel * labelLen    =GTK_LABEL(gtk_builder_get_object(builder,"labelLen"));
  GtkLabel * labelLenUnit    =GTK_LABEL(gtk_builder_get_object(builder,"labelLenUnit"));
  const gchar* text_entryR =  gtk_entry_get_text(entryR);
  const gchar* text_entryC  =  gtk_entry_get_text(entryC);
  long double d_R=0;
  long double d_C=0;
  long double mul;
  long double Pulsetime;
  const gchar* entry_combo;
  char * conv= (char*) calloc(11,sizeof(char));;
  char * unit_tmpl = (char*) calloc(4,sizeof(char));
  strcpy(unit_tmpl,time_template);
  if(!(strlen(text_entryR)*strlen(text_entryC)))
    return(FALSE);

  errno = 0;
  conv = (char*) calloc(gtk_entry_get_text_length(entryR)+1,sizeof(char));
  strncpy(conv,text_entryR,gtk_entry_get_text_length(entryR));
  d_R =  strtod(conv,NULL);
  free(conv);
  if(errno) return(FALSE);

  errno = 0;
  conv = (char*) calloc(gtk_entry_get_text_length(entryC)+1,sizeof(char));
  strncpy(conv,text_entryC,gtk_entry_get_text_length(entryC));
  d_C =  strtod(conv,NULL);
  free(conv);
  if(errno) return(FALSE);
  
  mul=1;
  entry_combo=gtk_combo_box_text_get_active_text(UnitR);
  if(!strcmp("Ω",entry_combo)) mul = 1;
  if(!strcmp("kΩ",entry_combo)) mul = 1e3;
  if(!strcmp("MΩ",entry_combo)) mul = 1e6;
  d_R*=mul;

  mul=1;
  entry_combo=gtk_combo_box_text_get_active_text(UnitC);
  if(!strcmp("pF",entry_combo)) mul = 1e-12;
  if(!strcmp("nF",entry_combo)) mul = 1e-9;
  if(!strcmp("µF",entry_combo)) mul = 1e-6; 
  if(!strcmp("mF",entry_combo)) mul = 1e-3; 
  if(!strcmp("F",entry_combo))  mul = 1; 
  d_C*=mul;
  
  conv = (char*) calloc(11,sizeof(char)); 
  Pulsetime = M_LN3 * d_R * d_C;
  waveDrawParams_MONOStable.high = Pulsetime;
  waveDrawParams_MONOStable.low=0;

  getSIPrefix(Pulsetime);
  Pulsetime = SIPrefix_ret.value;

  sprintf(conv,"%10.10Lf",Pulsetime);
 
  

  printf("%s\n",conv);
  fflush(NULL);
  *(conv+10)=0;
  *unit_tmpl = SIPrefix_ret.prefix;

  gtk_label_set_text (labelLen,conv);
  gtk_label_set_text (labelLenUnit,unit_tmpl);

  
   
  free(unit_tmpl);
  free(conv);
  gtk_widget_queue_draw (GTK_WIDGET(gtk_builder_get_object(builder,"drawingareaMONOStable")));
  return FALSE;
}

static gboolean Astable_inputChanged(GtkWidget *widget, GdkEvent *event, gpointer data){
 
  GtkEntry * entryR1 = GTK_ENTRY(gtk_builder_get_object(builder,"entryAStableR1")) ;
  GtkEntry * entryR2 = GTK_ENTRY(gtk_builder_get_object(builder,"entryAStableR2")); 
  GtkEntry * entryC  = GTK_ENTRY(gtk_builder_get_object(builder,"entryAStableC")); 
  GtkComboBoxText * UnitR1 = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder,"comboboxUnitR1"));
  GtkComboBoxText * UnitR2 = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder,"comboboxUnitR2"));
  GtkComboBoxText * UnitC  = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder,"comboboxUnitC"));
 
  GtkLabel * labelFreq    =GTK_LABEL(gtk_builder_get_object(builder,"labelCalcFreq"));
  GtkLabel * labelFreqUnit=GTK_LABEL(gtk_builder_get_object(builder,"labelCalcFreqUnit"));

  GtkLabel * labelDuty    =GTK_LABEL(gtk_builder_get_object(builder,"labelCalcDuty"));

  const gchar* text_entryR1 =  gtk_entry_get_text(entryR1);
  const gchar* text_entryR2 =  gtk_entry_get_text(entryR2);
  const gchar* text_entryC  =  gtk_entry_get_text(entryC);

  const gchar* entry_combo;

  char* conv;

  long double d_R1=0;
  long double d_R2=0;
  long double d_C=0;
  long double mul;
  int det =0;
  
  if(!(strlen(text_entryR1)*strlen(text_entryR2)*strlen(text_entryC)))
    return(FALSE);
  

#ifdef DEBUG_ASTABLE
   printf("%s\n",gtk_combo_box_text_get_active_text(UnitR1));
#endif
  errno = 0;
  conv = (char*) calloc(gtk_entry_get_text_length(entryR1)+1,sizeof(char));
  strncpy(conv,text_entryR1,gtk_entry_get_text_length(entryR1));
  d_R1 =  strtod(conv,NULL);
  // d_R1 =  atof(conv);
  free(conv);
  if(errno) return(FALSE);
#ifdef DEBUG_ASTABLE
  printf("Sr1:%s\n",conv);
#endif
  
  

  errno = 0;
  conv = (char*) calloc(gtk_entry_get_text_length(entryR2)+1,sizeof(char));
  strncpy(conv,text_entryR2,gtk_entry_get_text_length(entryR2));
  d_R2 =   strtod(conv,NULL);
  free(conv);
  if(errno) return(FALSE);
#ifdef DEBUG_ASTABLE
  printf("Sr2:%s\n",conv);
#endif
  


  errno = 0;
  conv = (char*) calloc(gtk_entry_get_text_length(entryC)+1,sizeof(char));
  strncpy(conv,text_entryC,gtk_entry_get_text_length(entryC));
  d_C =   strtod(conv,NULL);
  free(conv);
  if(errno) return(FALSE);
#ifdef DEBUG_ASTABLE
  printf("SC:%s\n",conv);
#endif
 
  
 

  entry_combo=gtk_combo_box_text_get_active_text(UnitR1);
  if(!strcmp("Ω",entry_combo)) mul = 1;
  if(!strcmp("kΩ",entry_combo)) mul = 1e3;
  if(!strcmp("MΩ",entry_combo)) mul = 1e6;
   d_R1*=mul;
  mul=1;
  entry_combo=gtk_combo_box_text_get_active_text(UnitR2);
  if(!strcmp("Ω",entry_combo)) mul = 1;
  if(!strcmp("kΩ",entry_combo)) mul = 1e3;
  if(!strcmp("MΩ",entry_combo)) mul = 1e6;
   d_R2*=mul;
  mul=1;
  entry_combo=gtk_combo_box_text_get_active_text(UnitC);
  if(!strcmp("pF",entry_combo)) mul = 1e-12;
  if(!strcmp("nF",entry_combo)) mul = 1e-9;
  if(!strcmp("µF",entry_combo)) mul = 1e-6; 
  if(!strcmp("mF",entry_combo)) mul = 1e-3; 
  if(!strcmp("F",entry_combo))  mul = 1; 
   d_C*=mul;

#ifdef DEBUG_ASTABLE
  printf("D\nr1:%Lf\nr2:%Lf\nC:%Lf\n",d_R1,d_R2,d_C);
#endif
  //g_print ("Change\n");

  long double freq = 1/(M_LN2 * d_C * (d_R1 + 2* d_R2));

  long double hightime = M_LN2 * d_C * (d_R1 + d_R2);
  long double lowtime  = M_LN2 * d_C * d_R2;

  waveDrawParams_AStable.high = hightime;
  waveDrawParams_AStable.low = lowtime;

  long double duty_perc= (hightime/(hightime+lowtime))*100.0;
  char * unit_tmpl = (char*) calloc(4,sizeof(char));
  strcpy(unit_tmpl,frequ_template);
  
  conv = (char*) calloc(11,sizeof(char));
  
  getSIPrefix(freq);
  freq = SIPrefix_ret.value;

  sprintf(conv,"%.10Lf",freq);

#ifdef DEBUG_ASTABLE
    printf("%s",conv);
#endif

    *(conv+10)=0;

    * unit_tmpl = SIPrefix_ret.prefix;

    

    gtk_label_set_text (labelFreq,conv);
    gtk_label_set_text (labelFreqUnit,unit_tmpl);

    sprintf(conv,"%10Lf",duty_perc);
    *(conv+10)=0;
    gtk_label_set_text (labelDuty,conv);

    free(unit_tmpl);
    free(conv);
    gtk_widget_queue_draw (GTK_WIDGET(gtk_builder_get_object(builder,"drawingareaAStable")));
    
  return FALSE;
}

char about_shown = FALSE;
gint cycleAbout ( GtkWidget *widget, GdkEvent  *event, gpointer   callback_data ){
  gtk_dialog_run (GTK_DIALOG (gtk_builder_get_object(builder,"aboutdialog2")));
  gtk_widget_destroy (GTK_DIALOG (gtk_builder_get_object(builder,"aboutdialog2")));
}

int main(int argc,char** argv){


  GtkWidget * window;
  GtkWidget * menubar;
  GtkWidget * grid;
  GtkWidget * button;
  GtkWidget * notebook;
  GtkWidget * widget;
  GtkDrawingArea * DrawAr;

  waveDrawParams_AStable.high=waveDrawParams_MONOStable.high=0;
  waveDrawParams_AStable.low =waveDrawParams_MONOStable.low =0;
  gtk_init(&argc,&argv);

  builder = gtk_builder_new();
  
  gtk_builder_add_from_file(builder,"555calc.glade.gui",NULL);

  window  = GTK_WIDGET (gtk_builder_get_object(builder,"window"));
  g_signal_connect (window,"destroy",G_CALLBACK(gtk_main_quit),NULL);  

  grid    = GTK_WIDGET (gtk_builder_get_object(builder,"main_grid"));
  menubar = GTK_WIDGET (gtk_builder_get_object(builder,"app_menubar"));
 
  button = GTK_WIDGET (gtk_builder_get_object(builder,"imagemenuFileQuit"));
  g_signal_connect (button,"activate",G_CALLBACK(gtk_main_quit),NULL);  

  button = GTK_WIDGET (gtk_builder_get_object(builder,"imagemenuAbout"));
  g_signal_connect (button,"activate",G_CALLBACK(cycleAbout),NULL);  
  button = GTK_WIDGET (gtk_builder_get_object(builder,"buttonAbout"));
  g_signal_connect (button,"activate",G_CALLBACK(cycleAbout),NULL); 

  notebook = GTK_WIDGET (gtk_builder_get_object(builder,"notebook1"));
  //gtk_notebook_set_current_page (GTK_NOTEBOOK(notebook),0);

  widget = GTK_WIDGET (gtk_builder_get_object(builder,"entryAStableC"));
  g_signal_connect (widget,"changed",G_CALLBACK(Astable_inputChanged),NULL);
  widget = GTK_WIDGET (gtk_builder_get_object(builder,"entryAStableR1"));
  g_signal_connect (widget,"changed",G_CALLBACK(Astable_inputChanged),NULL);
  widget = GTK_WIDGET (gtk_builder_get_object(builder,"entryAStableR2"));
  g_signal_connect (widget,"changed",G_CALLBACK(Astable_inputChanged),NULL);
  widget = GTK_WIDGET(gtk_builder_get_object(builder,"comboboxUnitR1"));
  g_signal_connect (widget,"changed",G_CALLBACK(Astable_inputChanged),NULL);
  widget = GTK_WIDGET(gtk_builder_get_object(builder,"comboboxUnitR2"));
  g_signal_connect (widget,"changed",G_CALLBACK(Astable_inputChanged),NULL);
  widget = GTK_WIDGET(gtk_builder_get_object(builder,"comboboxUnitC"));
  g_signal_connect (widget,"changed",G_CALLBACK(Astable_inputChanged),NULL);

  widget = GTK_WIDGET(gtk_builder_get_object(builder,"comboboxtextMonostableUnitC"));
  g_signal_connect (widget,"changed",G_CALLBACK(Monostable_inputChanged),NULL);
  widget = GTK_WIDGET(gtk_builder_get_object(builder,"comboboxtextMonostableUnitR"));
  g_signal_connect (widget,"changed",G_CALLBACK(Monostable_inputChanged),NULL);
  widget = GTK_WIDGET(gtk_builder_get_object(builder,"entryMonostableR"));
  g_signal_connect (widget,"changed",G_CALLBACK(Monostable_inputChanged),NULL);
  widget = GTK_WIDGET(gtk_builder_get_object(builder,"entryMonostableC"));
  g_signal_connect (widget,"changed",G_CALLBACK(Monostable_inputChanged),NULL);
  
  DrawAr = GTK_DRAWING_AREA(gtk_builder_get_object(builder,"drawingareaAStable"));
  g_signal_connect (G_OBJECT(DrawAr),"draw",G_CALLBACK(DrawAStable),NULL);
  DrawAr = GTK_DRAWING_AREA(gtk_builder_get_object(builder,"drawingareaMONOStable"));
  g_signal_connect (G_OBJECT(DrawAr),"draw",G_CALLBACK(DrawMONOStable),NULL);

gtk_widget_show (window);    

  gtk_main();

  return 0;



}
