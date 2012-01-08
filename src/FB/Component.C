/*
 * SPHERE - Skeleton for PHysical and Engineering REsearch
 *
 * Copyright (c) RIKEN, Japan. All right reserved. 2004-2010
 *
 */

//@file Component.C
//@brief FlowBase CompoList class
//@author keno, FSI Team, VCAD, RIKEN

#include "Component.h"

//@fn void CompoList::setInitTemp(SKL_REAL key)
//@brief 初期温度の指定
void CompoList::setInitTemp(SKL_REAL key) {
  temp_init = key;
}

//@fn void CompoList::setStateCellMonitor(unsigned key)
//@brief セルモニタスイッチ ON/OFF
void CompoList::setStateCellMonitor(unsigned key) {
  var_u1 = key;
}

//@fn void CompoList::setOutflowType(unsigned key)
//@brief 流出速度のタイプを指定する
//@note V_AVERAGE | V_MINMAX
void CompoList::setOutflowType(unsigned key) {
  var_u1 = key;
}

//@fn void CompoList::setsetPrsUnit(unsigned key)
//@brief 圧力の単位を指定する
void CompoList::setPrsUnit(unsigned key) {
  var_u1 = key;
}

//@fn void CompoList::setDef(int key)
//@brief 指定セルを保持する
void CompoList::setDef(int key) {
  def = key;
}

//@fn void CompoList::setCompoBV_st(unsigned odr, int val)
//@brief コンポーネントのBV情報の始点を設定する
//@param odr 方向　(0-i, 1-j, 2-k)
//@param val 値
void CompoList::setCompoBV_st(unsigned odr, int val)
{
  st[odr] = val;
}

//@fn void CompoList::setCompoBV_ed(unsigned odr, int val)
//@brief コンポーネントのBV情報の終点を設定する
//@param odr 方向　(0-i, 1-j, 2-k)
//@param val 値
void CompoList::setCompoBV_ed(unsigned odr, int val)
{
  ed[odr] = val;
}

//@fn void CompoList::set_sw_V_profile(unsigned key)
//@brief 速度プロファイル指定モードの保持
void CompoList::set_sw_V_profile(unsigned key)
{
  usw = key;
}

//@fn void CompoList::set_sw_P_BCtype(unsigned key)
//@brief 圧力境界条件タイプ指定モードの保持
void CompoList::set_sw_P_BCtype(unsigned key)
{
  usw = key;
}

//@fn void CompoList::set_sw_HTmodeRef(unsigned key)
//@brief 熱伝達の参照指定モードの保持
void CompoList::set_sw_HTmodeRef(unsigned key)
{
  usw = key;
}

//@fn void CompoList::set_sw_HexDir (unsigned key)
//@brief 熱交換機の方向指定モードの保持
void CompoList::set_sw_HexDir (unsigned key)
{
  usw = key;
}

//@fn void CompoList::set_sw_Heatgen(unsigned key)
//@brief 発熱量指定モードの保持
void CompoList::set_sw_Heatgen(unsigned key)
{
  usw = key;
}

//@fn void CompoList::set_Pressure(SKL_REAL var)
//@brief 圧力値の保持
void CompoList::set_Pressure(SKL_REAL var)
{
  var1 = var;
}

//@fn void CompoList::set_Mon_Temp(SKL_REAL var)
//@brief モニタ温度の保持
void CompoList::set_Mon_Temp(SKL_REAL var)
{
  var_m = var;
}

//@fn void CompoList::set_Mon_Heatflux(SKL_REAL var)
//@brief モニタ熱流束の保持
void CompoList::set_Mon_Heatflux(SKL_REAL var)
{
  var_m = var;
}

//@fn void CompoList::set_Mon_Calorie(SKL_REAL var)
//@brief モニタ熱量の保持
void CompoList::set_Mon_Calorie(SKL_REAL var)
{
  var_m = var;
}

//@fn void CompoList::set_CoefRadEps(SKL_REAL var)
//@brief 輻射のイプシロンの保持
void CompoList::set_CoefRadEps(SKL_REAL var)
{
  var1 = var;
}

//@fn void CompoList::CoefRadPrj(SKL_REAL var)
//@brief 輻射の射出係数の保持
void CompoList::set_CoefRadPrj(SKL_REAL var)
{
  var2 = var;
}

//@fn void CompoList::set_CoefMassflow(SKL_REAL var)
//@brief 流量の有次元化係数の保持
void CompoList::set_CoefMassflow(SKL_REAL var)
{
  var1 = var;
}

//@fn void CompoList::CoefPrsLoss(SKL_REAL var)
//@brief 圧力損失の有次元化係数の保持
void CompoList::set_CoefPrsLoss(SKL_REAL var)
{
  var2 = var;
}

//@fn void CompoList::set_HeatDensity(SKL_REAL var)
//@brief 吸発熱密度の保持
void CompoList::set_HeatDensity(SKL_REAL var)
{
  var3 = var;
}

//@fn void CompoList::set_HeatValue(SKL_REAL var)
//@brief 吸発熱量の保持
void CompoList::set_HeatValue(SKL_REAL var)
{
  var2 = var;
}

//@fn void CompoList::set_Temp(SKL_REAL var)
//@brief 温度の保持
void CompoList::set_Temp(SKL_REAL var)
{
  var3 = var;
}

//@fn void CompoList::set_CoefHT(SKL_REAL var)
//@brief 熱伝達係数の保持
void CompoList::set_CoefHT(SKL_REAL var)
{
  var2 = var;
}

//@fn void CompoList::set_Heatflux(SKL_REAL var)
//@brief 熱流束の保持
void CompoList::set_Heatflux(SKL_REAL var)
{
  var2 = var;
}

//@fn void CompoList::set_Massflow(SKL_REAL var)
//@brief 流量の保持
void CompoList::set_Massflow(SKL_REAL var)
{
  var1 = var;
}

//@fn void CompoList::set_Velocity(SKL_REAL var)
//@brief 速度の保持
void CompoList::set_Velocity(SKL_REAL var)
{
  var1 = var;
}

/**
 @fn bool CompoList::isMONITOR(void) const
 @brief コンポーネントタイプがモニタかどうかを調べる
 @retval モニタであればtrue
 */
bool CompoList::isMONITOR(void) const
{
  return ( (type == CELL_MONITOR) ? true : false );
}

/**
 @fn bool CompoList::isVBC(void) const
 @brief 内部境界条件タイプが速度指定かどうかを調べる
 @retval VBCであればtrue
 */
bool CompoList::isVBC(void) const
{
  if ((type == SPEC_VEL) ||
      (type == SPEC_VEL_WH) ||
      (type == OUTFLOW) ||
      (type == IBM_DF) ||
      (type == HEX) ||
      (type == FAN) ||
      (type == DARCY) ) return true;
  return false;
}

/**
 @fn bool CompoList::isFORCING(void) const
 @brief 境界条件タイプがFORCINGかどうかを調べる
 @retval FORCINGであればtrue
 */
bool CompoList::isFORCING(void) const
{
  if ((type == HEX) || 
      (type == FAN) || 
      (type == DARCY) ) return true;
  return false;
}

/**
 @fn bool CompoList::isHsrc(void) const
 @brief 境界条件タイプが熱源かどうかを調べる
 @retval FORCINGであればtrue
 */
bool CompoList::isHsrc(void) const
{
  if ((type == HEAT_SRC) || 
      (type == CNST_TEMP) ) return true;
  return false;
}

/**
 @fn bool CompoList::isHBC(void) const
 @brief 境界条件タイプが熱境界条件かどうかを調べる
 @retval HBCであればtrue
 */
bool CompoList::isHBC(void) const
{
  if ((type == ADIABATIC)  || 
      (type == HEATFLUX)   ||
      (type == TRANSFER)   ||
      (type == ISOTHERMAL) ||
      (type == RADIANT)    ||
      (type == SPEC_VEL_WH)||
      (type == HEAT_SRC)   ||
      (type == CNST_TEMP) ) return true;
  return false;
}

/**
 @fn bool CompoList::isVFraction(void) const
 @brief 体積率の必要なコンポーネントかどうか
 */
bool CompoList::isVFraction(void) const
{
  if ((type == HEAT_SRC) ||
      (type == CNST_TEMP) ||
      (type == IBM_DF) ||
      (type == HEX) ||
      (type == FAN) ||
      (type == DARCY) )  return true;
  return false;
}

//@fn void CompoList::addVec(SKL_REAL* vec)
//@brief ベクトルのコピー
void CompoList::addVec(SKL_REAL* vec) {
  nv[0] = vec[0];
  nv[1] = vec[1];
  nv[2] = vec[2];
}

//@fn void CompoList::setPeriodicDir(unsigned key)
//@brief 周期境界の上流方向を保持する
void CompoList::setPeriodicDir(unsigned key) {
  var_u1 = key;
}

/**
 @fn void CompoList::void setEns(bool key)
 @brief コンポーネントが自ノードに存在しているかどうかをセットする
 */
void CompoList::setEns(unsigned key) {
  ens = key;
}

/**
 @fn void CompoList::setPhase(unsigned m_phase)
 @brief set pahse ID (SOLID=0, FLUID=1, GAS=2, LIQUID=3)
 */
void CompoList::setPhase(unsigned m_phase) {
  phase = m_phase;
}

/**
 @fn void CompoList::setAttrb(unsigned key)
 @brief attrbをセットする
 @param key アトリビュート
 */
void CompoList::setAttrb(unsigned key) { attrb = key; }

//@fn void CompoList::set_VBC_policy(bool kind)
//@brief 速度指定ポリシーをセットする
//@param kind ポリシー種別　true-速度指定, false-流量指定
void CompoList::set_VBC_policy(bool kind)
{
  attrb = ( kind ) ? BC_type_velocity : BC_type_massflow;
}

//@fn void CompoList::set_HSRC_policy(bool kind)
//@brief 発熱項の指定ポリシーをセットする
//@param kind ポリシー種別　true-発熱量指定, false-発熱密度指定
void CompoList::set_HSRC_policy(bool kind)
{
  usw = ( kind ) ? hsrc_watt : hsrc_density;
}

/**
 @fn void CompoList::setElement(unsigned key)
 @brief elementをセットする
 @param key 要素数 element
 */
void CompoList::setElement(unsigned key) { element = key; }

/**
 @fn void CompoList::setID(unsigned key)
 @brief IDをセットする
 @param key 媒質ID ID
 */
void CompoList::setID(unsigned key) { ID = key; }

/**
 @fn void CompoList::setMatOdr (unsigned key)
 @brief mat_odrをセットする
 @param key MaterialListのエントリ番号
 */
void CompoList::setMatOdr(unsigned key) { mat_odr = key; }

/**
 @fn void CompoList::setState  (int key)
 @brief stateをセットする
 @param key セルの状態 SOLID/FLUID
 */
void CompoList::setState(int key) { state = key; }

/**
 @fn void CompoList::setType(int key)
 @brief typeをセットする
 @param key 境界条件の種類
 */
void CompoList::setType(unsigned key) { type = key; }

/**
 @fn void CompoList::setHtype(int key)
 @brief h_typeをセットする
 @param key 境界条件の種類
 */
void CompoList::setHtype(unsigned key) { h_type = key; }

/**
 @fn void CompoList::setName(const char* pnt)
 @brief ラベル名をセットする
 @param pnt ラベル名のアドレス
 @attention NULL check
 */
void CompoList::setName(const char* pnt) { strcpy(name, pnt); }

/**
 @fn bool CompoList::isVecForcing(void) const
 @brief ベクトル強制をするかどうかを調べる
 @retval ベクトルを強制する場合true
 */
bool CompoList::isVecForcing(void) const
{
  if ( isFORCING() ) {
    if ( usw==ON) return true;
  }
  return false;
}

/**
 @fn string CompoList::getVarStr(void) const
 @brief 変数名を返す
 @retval 変数の文字列
 */
string CompoList::getVarStr(void) const
{
  string var;
  
  if ( isVarEncoded(var_Velocity) )         var += "Velocity ";              // 0
  if ( isVarEncoded(var_Pressure) )         var += "Pressure ";              // 1
  if ( isVarEncoded(var_Temperature) )      var += "Temperature ";           // 2
  if ( isVarEncoded(var_Density) )          var += "Density ";               // 3
  if ( isVarEncoded(var_TotalP) )           var += "TotalPressure ";         // 4
  if ( isVarEncoded(var_Velocity_Avr) )     var += "Averaged Velocity ";     // 5
  if ( isVarEncoded(var_Pressure_Avr) )     var += "Averaged Pressure ";     // 6
  if ( isVarEncoded(var_Temperature_Avr) )  var += "Averaged Temperature ";  // 7
  if ( isVarEncoded(var_Density_Avr) )      var += "Averaged Density ";      // 8
  if ( isVarEncoded(var_TotalP_Avr) )       var += "Averaged TotalPressure ";// 9
  
  return var;
}

/**
 @fn string CompoList::getBCstr(void) const
 @brief BCのラベル名を返す
 */
string CompoList::getBCstr(void) const
{
  string bc;
  if      ( type == ADIABATIC )     bc = "Adiabatic";
  else if ( type == HEATFLUX )      bc = "Direct Heat Flux";
  else if ( type == ISOTHERMAL )    bc = "Isothermal";
  else if ( type == RADIANT )       bc = "Radiant";
  else if ( type == SPEC_VEL)       bc = "Specified_Velocity";
  else if ( type == SPEC_VEL_WH)    bc = "Specified_Velocity with Temperature";
  else if ( type == OUTFLOW)        bc = "Outflow";
  else if ( type == IBM_DF )        bc = "Immersed Boundary Method(Direct Forcing)";
  else if ( type == HEAT_SRC )      bc = "Heat Source";
  else if ( type == CNST_TEMP )     bc = "Constant Temperature";
  else if ( type == HEX )           bc = "Pressure Loss";
  else if ( type == FAN )           bc = "Fan";
  else if ( type == DARCY )         bc = "Darcy";
  else if ( type == CELL_MONITOR )  bc = "Cell_Monitor";
  else if ( type == PERIODIC )      bc = "Periodic";
  else if ( type == INACTIVE )      bc = "Inactive";
  else if ( type == TRANSFER ) {
    if      ( h_type == HT_N )          bc = "Heat Transfer type N";
    else if ( h_type == HT_S )          bc = "Heat Transfer type S";
    else if ( h_type == HT_SN)          bc = "Heat Transfer type SN (Natural convection)";
    else if ( h_type == HT_SF)          bc = "Heat Transfer type SF (Forced convection)";
    else if ( h_type == HT_B )          bc = "Heat Transfer type B";
  }
  else                            bc = "";
  return bc;
}