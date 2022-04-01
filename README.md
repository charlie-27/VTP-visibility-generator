
# VTP visibility generator

 - input file: `in.txt`
 - output file: `out.txt`
 - encoding: `Windows-1252`

### Input file example

```
R_key=kySET_1_Param;
R_var=MODBUS_HLDREG; R_addr=$0601;
R_for=SIGN; R_dec=1; R_inf=-990; R_sup=990; MU="#T";
Desc="LIDSetpoint";
DescEng="Set point temperature";
DescIta="Set point temperatura";
Shortdesc="SPt";
R_do=RWW;
ricsKey=SETPOINT1;
group=MAIN_PARAM;
<<<
R_key=kySPH;
R_var=MODBUS_HLDREG; R_addr=$0604;
R_for=SIGN; R_inf=0; R_sup=100; MU="#T";
Desc="";
DescEng="Set point RH";
DescIta="Set Regolazione RH";(* TODO
Shortdesc="SPH";
R_do=RWW;
<<<

R_key=kyPrG;
R_var=MODBUS_HLDREG; R_addr=$0602;
R_for=INT; R_inf=0; R_sup=2;
R_do=RWW;
group=MAIN_PARAM;
hidden=1;
R_infOptimum=0; R_supOptimum=0; R_infOk=0; R_supOk=0;
<<<
```

### Output file example

```
R_key=kySET_1_Param_VISIBILITY;
R_var=MODBUS_HLDREG; R_addr=$1001;
Desc="LIDVisibility..SET_1_Param";
DescEng="SET_1_Param visibility";
DescIta="Visibilità SET_1_Param";
<<<
R_key=kySPH_VISIBILITY;
R_var=MODBUS_HLDREG; R_addr=$1004;
Desc="LIDVisibility..SPH";
DescEng="SPH visibility";
DescIta="Visibilità SPH";
<<<
R_key=kyPrG_VISIBILITY;
R_var=MODBUS_HLDREG; R_addr=$1002;
Desc="LIDVisibility..PrG";
DescEng="PrG visibility";
DescIta="Visibilità PrG";
hidden=1;
<<<
```

### Note

 - Lo script prende l'indirizzo modbus del file di partenza e ci aggiunge `0xA00`;
 - gestisce anche parametri nascosti
