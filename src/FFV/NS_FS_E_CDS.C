//##################################################################################
//
// FFV-C : Frontflow / violet Cartesian
//
// Copyright (c) 2007-2011 VCAD System Research Program, RIKEN.
// All rights reserved.
//
// Copyright (c) 2011-2014 Institute of Industrial Science, The University of Tokyo.
// All rights reserved.
//
// Copyright (c) 2012-2014 Advanced Institute for Computational Science, RIKEN.
// All rights reserved.
//
//##################################################################################

/**
 * @file   NS_FS_E_CDS.C
 * @brief  FFV Class
 * @author aics
 */

#include "ffv.h"


// Fractional Step法でNavier-Stokes方程式を解く．距離情報近似．
void FFV::NS_FS_E_CDS()
{
  // local variables
  double flop;                         /// 浮動小数演算数
  double rhs_nrm = 0.0;                /// 反復解法での定数項ベクトルのノルム
  double res_init = 0.0;               /// 反復解法での初期残差ベクトルのL2ノルム
  double convergence=0.0;              /// 定常収束モニター量
  
  REAL_TYPE dt = deltaT;               /// 時間積分幅
  REAL_TYPE dh = (REAL_TYPE)deltaX;    /// 空間幅
  REAL_TYPE coef = deltaX/dt;          /// Poissonソース項の係数
  REAL_TYPE Re = C.Reynolds;           /// レイノルズ数
  REAL_TYPE rei = C.getRcpReynolds();  /// レイノルズ数の逆数
  REAL_TYPE half = 0.5;                /// 定数
  REAL_TYPE one = 1.0;                 /// 定数
  REAL_TYPE zero = 0.0;                /// 定数
  int wall_prof = C.Mode.Wall_profile; /// 壁面条件（slip/noslip）
  int cnv_scheme = C.CnvScheme;        /// 対流項スキーム
  
  
  // 境界処理用
  Gemini_R* m_buf = new Gemini_R [C.NoCompo+1];
  REAL_TYPE* m_snd = new REAL_TYPE [(C.NoCompo+1)*2];
  REAL_TYPE* m_rcv = new REAL_TYPE [(C.NoCompo+1)*2];

  
  int v_mode=0;
  
  IterationCtl* ICp = &IC[ic_prs1];  /// 圧力のPoisson反復
  IterationCtl* ICv = &IC[ic_vel1];  /// 粘性項のCrank-Nicolson反復
  IterationCtl* ICd = &IC[ic_div];   /// 圧力-速度反復
  
  // point Data
  // d_v   セルセンタ速度 v^n -> v^{n+1}
  // d_v0  セルセンタ速度 v^nの保持
  // d_vc  疑似速度ベクトル
  // d_wv  ワーク　陰解法の時の疑似速度ベクトル，射影ステップの境界条件
  // d_p   圧力 p^n -> p^{n+1}
  // d_p0  圧力 p^nの保持
  // d_ws  Poissonのソース項0　速度境界を考慮
  // d_sq Poissonのソース項1　反復毎に変化するソース項，摩擦速度，発散値, div(u)の値を保持，出力のところまでは再利用しないこと
  // d_bcd IDのビットフラグ
  // d_bcp 圧力のビットフラグ
  // d_cdf Component Directional BC Flag
  // d_wo  ワーク　壁関数利用時のWSS，ベクトル出力時のテンポラリ
  // d_cvf コンポーネントの体積率
  // d_ie0 内部エネルギー
  // d_vt  LES計算の渦粘性係数
  // d_ab0 Adams-Bashforth用のワーク
  // d_cut カット情報
  
  
  // >>> Fractional step section
  TIMING_start(tm_frctnl_stp_sct);
  
  // >>> Fractional step sub-section 1
  TIMING_start(tm_frctnl_stp_sct_1); 
  
  
  // n stepの値を保持 >> In use (d_v0, d_p0)
  TIMING_start(tm_copy_array);
  U.copyS3D(d_p0, size, guide, d_p, one);
  U.copyV3D(d_v0, size, guide, d_v, one);
  TIMING_stop(tm_copy_array, 0.0, 2);
  
  
  // 壁関数指定時の摩擦速度の計算 src0をテンポラリのワークとして利用
  if ( C.Mode.Wall_profile == Control::Log_Law )
  {
    TIMING_start(tm_WallFunc);
    flop = 0.0;
    friction_velocity_(d_ws, size, &guide, &dh, &Re, d_v0, d_bcp, range_Yp, range_Ut, v00, &flop);
    TIMING_stop(tm_WallFunc, flop);
  }
  
  TIMING_stop(tm_frctnl_stp_sct_1, 0.0);
  // <<< Fractional step subsection 1
  
  
 
  // >>> Fractional step sub-section 2
  TIMING_start(tm_frctnl_stp_sct_2);
  
  // 対流項と粘性項の評価 >> In use (dc_vc, dc_wv)
  switch (C.AlgorithmF) {
    case Flow_FS_EE_EE:
    case Flow_FS_AB2:
      TIMING_start(tm_pseudo_vec);
      
      flop = 0.0;
      v_mode = (C.Mode.Wall_profile == Control::Log_Law) ? 2 : 1;

      if ( C.LES.Calc == ON )
      {
        Hostonly_ printf("not inplemented yet. sorry:-)\n");
        Exit(0);
      }
      else
      {
        pvec_muscl_cds_(d_vc, size, &guide, &dh, &cnv_scheme, v00, &rei, d_v0, d_vf, d_cdf, d_bcp, &v_mode, d_cut, &flop);
      }
      TIMING_stop(tm_pseudo_vec, flop);

      TIMING_start(tm_pvec_flux);
      flop = 0.0;
      BC.modPvecFlux(d_vc, d_v0, d_cdf, CurrentTime, &C, v_mode, v00, flop);
      TIMING_stop(tm_pvec_flux, flop);
      break;
      
    case Flow_FS_AB_CN:
      TIMING_start(tm_pseudo_vec);
      flop = 0.0;
      v_mode = 0;
      if ( C.LES.Calc == ON )
      {
        Hostonly_ printf("not inplemented yet. sorry:-)\n");
      }
      else
      {
        pvec_muscl_cds_(d_wv, size, &guide, &dh, &cnv_scheme, v00, &rei, d_v0, d_vf, d_cdf, d_bcp, &v_mode, d_cut, &flop);
      }
      TIMING_stop(tm_pseudo_vec, flop);
      
      TIMING_start(tm_pvec_flux);
      flop = 0.0;
      BC.modPvecFlux(d_wv, d_v0, d_cdf, CurrentTime, &C, v_mode, v00, flop);
      TIMING_stop(tm_pvec_flux, flop);
      break;
      
    default:
      Exit(0);
  }

  // 時間積分
  switch (C.AlgorithmF)
  {
    case Flow_FS_EE_EE:
      TIMING_start(tm_pvec_ee);
      flop = 0.0;
      euler_explicit_ (d_vc, size, &guide, &dt, d_v0, d_bcd, &flop);
      TIMING_stop(tm_pvec_ee, flop);
      break;
      
    case Flow_FS_AB2:
      TIMING_start(tm_pvec_ab);
      flop = 0.0;
      if ( Session_CurrentStep == 1 ) // 初期とリスタート後，1ステップめ
      {
        euler_explicit_ (d_vc, size, &guide, &dt, d_v0, d_bcd, &flop);
      }
      else
      {
        ab2_(d_vc, size, &guide, &dt, d_v0, d_abf, d_bcd, v00, &flop);
      }
      TIMING_stop(tm_pvec_ab, flop);
      break;
      
    case Flow_FS_AB_CN: // 未対応20110918
      TIMING_start(tm_pvec_abcn);
      flop = 0.0;
      if ( Session_CurrentStep == 1 )
      {
        euler_explicit_ (d_wv, size, &guide, &dt, d_v0, d_bcd, &flop);
      }
      else
      {
        ab2_(d_wv, size, &guide, &dt, d_v0, d_abf, d_bcd, v00, &flop);
      }
      TIMING_stop(tm_pvec_abcn, flop);
      
      TIMING_start(tm_pvec_abcn_df_ee);
      flop = 0.0;
      vis_ee_(d_vc, size, &guide, &dh, &dt, v00, &rei, d_wv, d_v0, d_cdf, &half, &flop);
      TIMING_stop(tm_pvec_abcn_df_ee, flop);
      
      TIMING_start(tm_pvec_abcn_df_ee_BC);
      flop = 0.0;
      BC.mod_Vis_EE(d_vc, d_v0, half, d_cdf, CurrentTime, dt, v00, flop);
      TIMING_stop(tm_pvec_abcn_df_ee_BC, flop);
      break;
      
    default:
      Exit(0);
  }
  
  TIMING_stop(tm_frctnl_stp_sct_2, 0.0);
  // <<< Fractional step subsection 2
  

  
  // >>> Fractional step sub-section 3
  TIMING_start(tm_frctnl_stp_sct_3);
  
  // FORCINGコンポーネントの疑似速度ベクトルの方向修正
  if ( C.EnsCompo.forcing == ON )
  {
    TIMING_start(tm_forcing);
    flop = 0.0;
    BC.mod_Pvec_Forcing(d_vc, d_v, d_bcd, d_cvf, v00, dt, flop);
    TIMING_stop(tm_forcing, flop);
  }
  
  // 浮力項
  if ( C.isHeatProblem() && (C.Mode.Buoyancy == BOUSSINESQ) )
  {
    TIMING_start(tm_buoyancy);
    REAL_TYPE dgr = dt*C.Grashof*rei*rei;
    flop = 3.0;
    Buoyancy(d_vc, dgr, d_ie0, d_bcd, flop);
    TIMING_stop(tm_buoyancy, flop);
  }
  
  
  // 疑似ベクトルの境界条件
  TIMING_start(tm_pvec_BC);
  BC.OuterVBCpseudo(d_vc, d_cdf, &C);
  BC.InnerVBCperiodic(d_vc, d_bcd);
  TIMING_stop(tm_pvec_BC, 0.0);
  
  
  // 疑似ベクトルの同期
  if ( numProc > 1 )
  {
    TIMING_start(tm_pvec_comm);
    if ( paraMngr->BndCommV3D(d_vc, size[0], size[1], size[2], guide, 1) != CPM_SUCCESS ) Exit(0);
    TIMING_stop(tm_pvec_comm, face_comm_size*1.0*3.0); // ガイドセル数 x ベクトル
  }
  
  TIMING_stop(tm_frctnl_stp_sct_3, 0.0);
  // <<< Fractional step subsection 3
  

  
  // >>> Fractional step sub-section 4
  TIMING_start(tm_frctnl_stp_sct_4);
  
  // Crank-Nicolson Iteration
  if ( C.AlgorithmF == Flow_FS_AB_CN )
  {
    TIMING_start(tm_copy_array);
    U.copyV3D(d_wv, size, guide, d_vc, one);
    TIMING_stop(tm_copy_array, 0.0);
    
    for (ICv->setLoopCount(0); ICv->getLoopCount()< ICv->getMaxIteration(); ICv->incLoopCount())
    {
      //CN_Itr(ICv);
      if (  ICv->isConverged() ) break;
    }
  }
  
  TIMING_stop(tm_frctnl_stp_sct_4, 0.0);
  // <<< Fractional step subsection 4
  
  TIMING_stop(tm_frctnl_stp_sct, 0.0);
  // <<< Fractional step section
  

  
  // Poissonのソース部分
  // >>> Poisson Source section
  TIMING_start(tm_poi_src_sct);
  
  
  // vの初期値をvcにしておく
  TIMING_start(tm_copy_array);
  U.copyV3D(d_v, size, guide, d_vc, one);
  TIMING_stop(tm_copy_array, 0.0);
  
  
  // 非反復ソース項のゼロクリア src0
  TIMING_start(tm_assign_const);
  U.initS3D(d_ws, size, guide, zero);
  TIMING_stop(tm_assign_const, 0.0);
  
  
  // 非VBC面に対してのみ，セルセンターの値から発散量を計算
  TIMING_start(tm_div_pvec);
  flop = 0.0;
  divergence_cds_(d_ws, size, &guide, &coef, d_vc, d_cdf, d_cut, v00, &flop);
  TIMING_stop(tm_div_pvec, flop);
  
  
  // Poissonソース項の速度境界条件（VBC）面による修正
  TIMING_start(tm_poi_src_vbc);
  flop = 0.0;
  BC.modPsrcVBC(d_ws, d_vc, d_v0, d_vf, d_cdf, CurrentTime, dt, &C, v00, flop);
  TIMING_stop(tm_poi_src_vbc, flop);
  
  
  // (Neumann_BCType_of_Pressure_on_solid_wall == grad_NS)　のとき，\gamma^{N2}の処理
  //hogehoge
  
  // 定数項のL2ノルム　rhs_nrm
  if ( (ICp->getNormType() == dx_b) || (ICp->getNormType() == r_b) )
  {
    TIMING_start(tm_poi_src_nrm);
    rhs_nrm = 0.0;
    flop = 0.0;
    if (ICp->getBit3() == OFF)
    {
      poi_rhs_(&rhs_nrm, d_b, size, &guide, d_ws, d_sq, d_bcp, &dh, &dt, &flop);
    }
    else
    {
      poi_rhs_bit3_(&rhs_nrm, d_b, size, &guide, d_ws, d_sq, d_bcp, &dh, &dt, &flop);
    }
    TIMING_stop(tm_poi_src_nrm, flop);
    
    if ( numProc > 1 )
    {
      TIMING_start(tm_poi_src_comm);
      double m_tmp = rhs_nrm;
      if ( paraMngr->Allreduce(&m_tmp, &rhs_nrm, 1, MPI_SUM) != CPM_SUCCESS ) Exit(0);
      TIMING_stop(tm_poi_src_comm, 2.0*numProc*sizeof(double) ); // 双方向 x ノード数
    }
    
    rhs_nrm = sqrt(rhs_nrm);
  }
  
  // Initial residual
  if ( ICp->getNormType() == r_r0 )
  {
    TIMING_start(tm_poi_src_nrm);
    res_init = 0.0;
    flop = 0.0;
    if (ICp->getBit3() == OFF)
    {
      poi_residual_(&res_init, size, &guide, d_p, d_b, d_bcp, &flop);
    }
    else
    {
      poi_residual_bit3_(&res_init, size, &guide, d_p, d_b, d_bcp, &flop);
    }
    TIMING_stop(tm_poi_src_nrm, flop);
    
    if ( numProc > 1 )
    {
      TIMING_start(tm_poi_src_comm);
      double m_tmp = res_init;
      if ( paraMngr->Allreduce(&m_tmp, &res_init, 1, MPI_SUM) != CPM_SUCCESS ) Exit(0);
      TIMING_stop(tm_poi_src_comm, 2.0*numProc*sizeof(double) ); // 双方向 x ノード数
    }
    
    res_init = sqrt(res_init);
  }
  
  TIMING_stop(tm_poi_src_sct, 0.0);
  // <<< Poisson Source section
  
  
  
  // VP-Iteration
  // >>> Poisson Iteration section
  TIMING_start(tm_poi_itr_sct);
  
  if ( C.Mode.Log_Itr == ON )
  {
    TIMING_start(tm_hstry_itr);
    Hostonly_ H->printHistoryItrTitle(fp_i);
    TIMING_stop(tm_hstry_itr, 0.0);
  }

  for (ICp->setLoopCount(0); ICp->getLoopCount() <= ICp->getMaxIteration(); ICp->incLoopCount())
  {
    
    // >>> Poisson Iteration subsection 1
    TIMING_start(tm_poi_itr_sct_1);
    
    // 反復ソース項のゼロクリア => src1
    TIMING_start(tm_assign_const);
    U.initS3D(d_sq, size, guide, zero);
    TIMING_stop(tm_assign_const, 0.0);
    
    // Forcingコンポーネントによるソース項の寄与分
    if ( C.EnsCompo.forcing == ON )
    {
      TIMING_start(tm_force_src);
      flop=0.0;
      BC.mod_Psrc_Forcing(d_sq, d_v, d_bcd, d_cvf, v00, component_array, flop);
      TIMING_stop(tm_force_src, flop);
    }
    
    // 内部周期境界部分のディリクレソース項
    //TIMING_start(tm_prdc_src);
    //BC.InnerPrdc_Src(dc_wk2, dc_p, dc_bcd);
    //TIMING_stop(tm_prdc_src, flop);
    
    TIMING_stop(tm_poi_itr_sct_1, 0.0);
    // <<< Poisson Iteration subsection 1

    // 線形ソルバー
    switch (ICp->getLS())
    {
      case SOR:
        Point_SOR(ICp, d_p, d_b, rhs_nrm, res_init); // return x^{m+1} - x^m
        break;
        
      case SOR2SMA:
        SOR_2_SMA(ICp, d_p, d_b, rhs_nrm, res_init); // return x^{m+1} - x^m
        break;
        
      case GMRES:
        Fgmres(ICp, rhs_nrm, res_init); // return ?
        break;
        
      default:
        printf("\tInvalid Linear Solver for Pressure\n");
        Exit(0);
        break;
    }
    

    // >>> Poisson Iteration subsection 4
    TIMING_start(tm_poi_itr_sct_4);
    
    // 速度のスカラポテンシャルによる射影と発散値 src1は，反復毎のソース項をワークとして利用
    TIMING_start(tm_prj_vec);
    flop = 0.0;
    update_vec_cds_(d_v, d_dv, size, &guide, &dt, &dh, d_vc, d_p, d_bcp, d_cdf, d_cut, v00, &flop);
    TIMING_stop(tm_prj_vec, flop);
    
    // セルフェイス速度の境界条件による修正
    TIMING_start(tm_prj_vec_bc);
    flop=0.0;
    BC.modDivergence(d_dv, d_cdf, CurrentTime, v00, m_buf, d_vf, d_v, &C, flop);
    TIMING_stop(tm_prj_vec_bc, flop);
    
    // セルフェイス速度の境界条件の通信部分
    if ( C.EnsCompo.outflow == ON )
    {
      if ( numProc > 1 )
      {
        for (int n=1; n<=C.NoCompo; n++)
        {
          m_snd[2*n]   = m_rcv[2*n]   = m_buf[n].p0; // 積算速度
          m_snd[2*n+1] = m_rcv[2*n+1] = m_buf[n].p1; // 積算回数
        }
        
        TIMING_start(tm_prj_vec_bc_comm);
        if ( paraMngr->Allreduce(m_snd, m_rcv, 2*(C.NoCompo+1), MPI_SUM) != CPM_SUCCESS ) Exit(0);
        TIMING_stop(tm_prj_vec_bc_comm, 2.0*(C.NoCompo+1)*numProc*sizeof(REAL_TYPE)*2.0 ); // 双方向 x ノード数 x 変数
        
        for (int n=1; n<=C.NoCompo; n++)
        {
          m_buf[n].p0 = m_rcv[2*n];
          m_buf[n].p1 = m_rcv[2*n+1];
        }
      }
      
      for (int n=1; n<=C.NoCompo; n++)
      {
        if ( cmp[n].getType() == OUTFLOW )
        {
          cmp[n].val[var_Velocity] = m_buf[n].p0 / m_buf[n].p1; // 無次元平均流速
        }
      }
    }
    
    // Forcingコンポーネントによる速度と発散値の修正
    if ( C.EnsCompo.forcing == ON )
    {
      TIMING_start(tm_prj_frc_mod);
      flop=0.0;
      BC.mod_Vdiv_Forcing(d_v, d_bcd, d_cvf, d_dv, dt, v00, m_buf, component_array, flop);
      TIMING_stop(tm_prj_frc_mod, flop);
      
      // 通信部分
      if ( numProc > 1 )
      {
        for (int n=1; n<=C.NoCompo; n++)
        {
          m_snd[2*n]   = m_rcv[2*n]   = m_buf[n].p0; // 積算速度
          m_snd[2*n+1] = m_rcv[2*n+1] = m_buf[n].p1; // 積算圧力損失
        }
        
        TIMING_start(tm_prj_frc_mod_comm);
        if ( paraMngr->Allreduce(m_snd, m_rcv, 2*(C.NoCompo+1), MPI_SUM) != CPM_SUCCESS ) Exit(0);
        TIMING_stop(tm_prj_frc_mod_comm, 2.0*(C.NoCompo+1)*numProc*sizeof(REAL_TYPE)*2.0);
        
        for (int n=1; n<=C.NoCompo; n++)
        {
          m_buf[n].p0 = m_rcv[2*n];
          m_buf[n].p1 = m_rcv[2*n+1];
        }
      }
      
      for (int n=1; n<=C.NoCompo; n++)
      {
        if ( cmp[n].isFORCING() )
        {
          REAL_TYPE aa = (REAL_TYPE)cmp[n].getElement();
          cmp[n].val[var_Velocity] = m_buf[n].p0 / aa; // 平均速度
          cmp[n].val[var_Pressure] = m_buf[n].p1 / aa; // 平均圧力損失量
        }
      }
    }

    // 周期型の速度境界条件
    TIMING_start(tm_vec_BC);
    BC.InnerVBCperiodic(d_v, d_bcd);
    TIMING_stop(tm_vec_BC, 0.0);
    
    TIMING_stop(tm_poi_itr_sct_4, 0.0);
    // <<< Poisson Iteration subsection 4
 
    
    // ノルムの計算
    Norm_Div(ICd);
    
    /* Forcingコンポーネントによる速度の方向修正(収束判定から除外)  >> TEST
     TIMING_start(tm_prj_frc_dir);
     flop=0.0;
     BC.mod_Dir_Forcing(d_v, d_bcd, d_cvf, v00, flop);
     TIMING_stop(tm_prj_frc_dir, flop);
     */
    
    // 収束判定　性能測定モードのときは収束判定を行わない
    if ( (C.Hide.PM_Test == OFF) && (convergence < ICp->getCriterion()) ) break;
  } // end of iteration
  
  TIMING_stop(tm_poi_itr_sct, 0.0);
  // <<< Poisson Iteration section
  
  
  
  /// >>> NS Loop post section
  TIMING_start(tm_NS_loop_post_sct);
  
  // 同期
  if ( numProc > 1 )
  {
    TIMING_start(tm_vectors_comm);
    if ( paraMngr->BndCommV3D(d_v, size[0], size[1], size[2], guide, guide) != CPM_SUCCESS ) Exit(0);
    TIMING_stop(tm_vectors_comm, 2*face_comm_size*guide*3.0);
  }
  
  // 外部領域境界面での速度や流量を計算 > 外部流出境界条件の移流速度に利用
  TIMING_start(tm_domain_monitor);
  DomainMonitor(BC.exportOBC(), &C);
  TIMING_stop(tm_domain_monitor, 0.0);
  
  
  // 非同期にして隠す
  if (C.LES.Calc==ON)
  {
    TIMING_start(tm_LES_eddy);
    flop = 0.0;
    eddy_viscosity_(d_vt, size, &guide, &dh, &C.Reynolds, &C.LES.Cs, d_v, d_cdf, range_Ut, range_Yp, v00);
    TIMING_stop(tm_LES_eddy, flop);
    
    if ( numProc > 1 )
    {
      TIMING_start(tm_LES_eddy_comm);
      if ( paraMngr->BndCommS3D(d_vt, size[0], size[1], size[2], guide, guide) != CPM_SUCCESS ) Exit(0);
      TIMING_stop(tm_LES_eddy_comm, face_comm_size*guide);
    }
  }
  
  // ノルムの増加率が規定値をこえたら，終了
  if (CM_F.previous != 0.0 )
  {
    CM_F.rate = convergence / CM_F.previous;
  }
  else
  {
    CM_F.rate = 1.0;
  }
  CM_F.previous = convergence;
  

  
  TIMING_stop(tm_NS_loop_post_sct, 0.0);
  // >>> NS loop post section
  
  // 後始末
  if ( m_buf ) delete [] m_buf;
  if ( m_snd ) delete [] m_snd;
  if ( m_rcv ) delete [] m_rcv;
  
}
