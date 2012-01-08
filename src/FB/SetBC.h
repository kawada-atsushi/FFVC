#ifndef _SKL_FB_SETBC_H_
#define _SKL_FB_SETBC_H_

/*
 * SPHERE - Skeleton for PHysical and Engineering REsearch
 *
 * Copyright (c) RIKEN, Japan. All right reserved. 2004-2010
 *
 */

//@file SetBC.h
//@brief FlowBase SetBC class Header
//@author keno, FSI Team, VCAD, RIKEN

#include "Skl.h"
#include "SklSolverBase.h"
#include "FBDefine.h"
#include "FBUtility.h"
#include "BndOuter.h"
#include "mydebug.h"
#include "Control.h"
#include "Component.h"
#include "Material.h"
#include "SklUtil.h"
#include "Intrinsic.h"
#include "Parallel_node.h"
#include "Ref_Frame.h"

class SetBC : public Parallel_Node {
protected:
  int *ix, *jx, *kx, *gc;
  int dim_sz[3], ixc, jxc, kxc;
  
  SKL_REAL dh, accel, Dp1, Dp2, mach, BasePrs;
  SKL_REAL RefV, RefL, DiffTemp, BaseTemp, Peclet, Reynolds, rei, pei;
  SKL_REAL Lbx[3], Rayleigh, Grashof, Prandtl;
  SKL_REAL rho, nyu, cp, lambda, beta;
  
  unsigned imax, jmax, kmax, guide, size[3];
  unsigned Example, NoBC, Unit_Temp, Unit_Prs;
  bool     inout_flag;
  
  BoundaryOuter   obc[NOFACE];
  CompoList*      cmp;
  MaterialList*   mat;
  Intrinsic*      Ex;
  
  void getOuterLoopIdx (int face, int* st, int* ed);
  
  /// 外部境界の種類
  enum obc_kind {
    id_specvel = 0,
    id_outflow,
    id_wall,
    id_symmetric,
    id_periodic
  };
  
public:
  SetBC() {
    imax = jmax = kmax = guide = 0;
    ix = jx = kx = gc = NULL;
    ixc = jxc = kxc = 0;
    dh = rei = accel = Dp1 = Dp2 = mach = RefV = RefL = DiffTemp = BaseTemp = pei = 0.0;
    rho = nyu = cp = lambda = beta = BasePrs = 0.0;
    Peclet = Reynolds = Rayleigh = Grashof = Prandtl = 0.0;
    Example = NoBC = Unit_Temp = Unit_Prs = 0;
    inout_flag = false;
    
    for (int i=0; i<3; i++) {
      dim_sz[i] = 0;
      size[i]   = 0.0;
      Lbx[i]    = 0.0;
    }
    
    cmp = NULL;
    mat = NULL;
    Ex  = NULL;
  }
  virtual ~SetBC() {}

public:
	SKL_REAL getVrefOut (SKL_REAL tm);
  
  void setControlVars (Control* Cref, MaterialList* mat, CompoList* cmp, ReferenceFrame* RF, Intrinsic* ExRef=NULL);
  void set_InOut_flag (void);
  void setWorkList    (CompoList* m_CMP, MaterialList* m_MAT);
  
  //@fn bool has_InOut(void)
  //@brief 流入出境界条件が設定されている場合にtrueを返す
  bool has_InOut(void) { return inout_flag; }
  
  //@fn BoundaryOuter* get_OBC_Ptr(void)
  //@brief 外部境界リストのポインタを返す
  BoundaryOuter* get_OBC_Ptr(void) { 
    return obc;
  }
  
  //@fn BoundaryOuter* get_OBC_Ptr(int face)
  //@brief 引数の外部境界面の外部境界リストのポインタを返す
  //@param face 面番号
  BoundaryOuter* get_OBC_Ptr(int face) { 
    return &obc[face];
  }
  
};

#endif // _SKL_FB_SETBC_H_