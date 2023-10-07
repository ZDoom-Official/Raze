class BloodDudeBase : Bloodactor
{
	default
	{
		AISTATE "genIdle", "+0", 1, 0, null, null, null, null, "none";
		AISTATE "genRecoil", "+5", 5, 20, null, null, null, null, "genIdle";
		AISTATE "batIdle", "+0", 0, 0, null, null, null, batThinkTarget, "none";
		AISTATE "batFlyIdle", "+6", 0, 0, null, null, null, batThinkTarget, "none";
		AISTATE "batChase", "+6", 4, 0, null, null, batMoveForward, batThinkChase, "batFlyIdle";
		AISTATE "batPonder", "+6", -1, 0, null, null, null, batThinkPonder, "none";
		AISTATE "batGoto", "+6", 2, 600, null, null, batMoveForward, batThinkGoto, "batFlyIdle";
		AISTATE "batBite", "+7", 4, 60, batBiteSeqCallback, null, null, null, "batPonder";
		AISTATE "batRecoil", "+5", 5, 0, null, null, null, null, "batChase";
		AISTATE "batSearch", "+6", 3, 120, null, null, batMoveForward, batThinkSearch, "batFlyIdle";
		AISTATE "batSwoop", "+6", -1, 60, null, null, batMoveSwoop, batThinkChase, "batChase";
		AISTATE "batFly", "+6", 2, 0, null, null, batMoveFly, batThinkChase, "batChase";
		AISTATE "batTurn", "+6", 2, 60, null, null, aiMoveTurn, null, "batChase";
		AISTATE "batHide", "+6", -1, 0, null, null, batMoveToCeil, batMoveForward, "none";
		AISTATE "batDodgeUp", "+6", 2, 120, null, null, batMoveDodgeUp, null, "batChase";
		AISTATE "batDodgeUpRight", "+6", 2, 90, null, null, batMoveDodgeUp, null, "batChase";
		AISTATE "batDodgeUpLeft", "+6", 2, 90, null, null, batMoveDodgeUp, null, "batChase";
		AISTATE "batDodgeDown", "+6", 2, 120, null, null, batMoveDodgeDown, null, "batChase";
		AISTATE "batDodgeDownRight", "+6", 2, 90, null, null, batMoveDodgeDown, null, "batChase";
		AISTATE "batDodgeDownLeft", "+6", 2, 90, null, null, batMoveDodgeDown, null, "batChase";
		AISTATE "beastIdle", "+0", 0, 0, null, null, null, aiThinkTarget, "none";
		AISTATE "beastChase", "+8", 4, 0, null, null, beastMoveForward, beastThinkChase, "none";
		AISTATE "beastDodge", "+8", 2, 60, null, null, aiMoveDodge, null, "beastChase";
		AISTATE "beastGoto", "+8", 2, 600, null, null, beastMoveForward, beastThinkGoto, "beastIdle";
		AISTATE "beastSlash", "+6", 4, 120, SlashSeqCallback, null, null, null, "beastChase";
		AISTATE "beastStomp", "+7", 4, 120, StompSeqCallback, null, null, null, "beastChase";
		AISTATE "beastSearch", "+8", 3, 120, null, null, beastMoveForward, beastThinkSearch, "beastIdle";
		AISTATE "beastRecoil", "+5", 5, 0, null, null, null, null, "beastDodge";
		AISTATE "beastTeslaRecoil", "+4", 5, 0, null, null, null, null, "beastDodge";
		AISTATE "beastSwimIdle", "+9", 0, 0, null, null, null, aiThinkTarget, "none";
		AISTATE "beastSwimChase", "+9", 4, 0, null, null, beastMoveSwim, beastThinkSwimChase, "none";
		AISTATE "beastSwimDodge", "+9", 2, 90, null, null, aiMoveDodge, null, "beastSwimChase";
		AISTATE "beastSwimGoto", "+9", 2, 600, null, null, beastMoveForward, beastThinkSwimGoto, "beastSwimIdle";
		AISTATE "beastSwimSearch", "+9", 3, 120, null, null, beastMoveForward, beastThinkSearch, "beastSwimIdle";
		AISTATE "beastSwimSlash", "+9", 4, 0, SlashSeqCallback, null, null, beastThinkSwimChase, "beastSwimChase";
		AISTATE "beastSwimRecoil", "+5", 5, 0, null, null, null, null, "beastSwimDodge";
		AISTATE "beastMorphFromCultist", "+2576", -1, 0, null, null, null, null, "beastMorphToBeast";
		AISTATE "beastMoveSwimChaseAlt", "+9", -1, 120, null, null, beastMoveSwimAlt, beastThinkSwimChase, "beastSwimChase";
		AISTATE "beastSwimAttack", "+9", -1, 0, null, null, beastMoveIn, beastThinkSwimChase, "beastSwimChase";
		AISTATE "beastSwimTurn", "+9", -1, 120, null, null, aiMoveTurn, null, "beastSwimChase";
		AISTATE "eelIdle", "+0", 0, 0, null, null, null, eelThinkTarget, "none";
		AISTATE "eelFlyIdle", "+0", 0, 0, null, null, null, eelThinkTarget, "none";
		AISTATE "eelChase", "+0", 4, 0, null, null, eelMoveForward, eelThinkChase, "eelIdle";
		AISTATE "eelPonder", "+0", -1, 0, null, null, null, eelThinkPonder, "none";
		AISTATE "eelGoto", "+0", 2, 600, null, null, null, eelThinkGoto, "eelIdle";
		AISTATE "eelBite", "+7", 4, 60, eelBiteSeqCallback, null, null, null, "eelChase";
		AISTATE "eelRecoil", "+5", 5, 0, null, null, null, null, "eelChase";
		AISTATE "eelSearch", "+0", 3, 120, null, null, eelMoveForward, eelThinkSearch, "eelIdle";
		AISTATE "eelSwoop", "+0", -1, 60, null, null, eelMoveSwoop, eelThinkChase, "eelChase";
		AISTATE "eelFly", "+0", 2, 0, null, null, eelMoveAscend, eelThinkChase, "eelChase";
		AISTATE "eelTurn", "+0", 2, 60, null, null, aiMoveTurn, null, "eelChase";
		AISTATE "eelHide", "+0", -1, 0, null, null, eelMoveToCeil, eelMoveForward, "none";
		AISTATE "eelDodgeUp", "+0", 2, 120, null, null, eelMoveDodgeUp, null, "eelChase";
		AISTATE "eelDodgeUpRight", "+0", 2, 90, null, null, eelMoveDodgeUp, null, "eelChase";
		AISTATE "eelDodgeUpLeft", "+0", 2, 90, null, null, eelMoveDodgeUp, null, "eelChase";
		AISTATE "eelDodgeDown", "+0", 2, 120, null, null, eelMoveDodgeDown, null, "eelChase";
		AISTATE "eelDodgeDownRight", "+0", 2, 90, null, null, eelMoveDodgeDown, null, "eelChase";
		AISTATE "eelDodgeDownLeft", "+0", 2, 90, null, null, eelMoveDodgeDown, null, "eelChase";
		AISTATE "cultistBurnIdle", "+3", 0, 0, null, null, null, aiThinkTarget, "none";
		AISTATE "cultistBurnChase", "+3", 4, 0, null, null, aiMoveForward, burnThinkChase, "none";
		AISTATE "cultistBurnGoto", "+3", 2, 3600, null, null, aiMoveForward, burnThinkGoto, "cultistBurnSearch";
		AISTATE "cultistBurnSearch", "+3", 3, 3600, null, null, aiMoveForward, burnThinkSearch, "cultistBurnSearch";
		AISTATE "cultistBurnAttack", "+3", 4, 120, BurnSeqCallback, null, null, null, "cultistBurnChase";
		AISTATE "zombieABurnChase", "+3", 4, 0, null, null, aiMoveForward, burnThinkChase, "none";
		AISTATE "zombieABurnGoto", "+3", 2, 3600, null, null, aiMoveForward, burnThinkGoto, "zombieABurnSearch";
		AISTATE "zombieABurnSearch", "+3", 3, 3600, null, null, aiMoveForward, burnThinkSearch, "none";
		AISTATE "zombieABurnAttack", "+3", 4, 120, BurnSeqCallback, null, null, null, "zombieABurnChase";
		AISTATE "zombieFBurnChase", "+3", 4, 0, null, null, aiMoveForward, burnThinkChase, "none";
		AISTATE "zombieFBurnGoto", "+3", 2, 3600, null, null, aiMoveForward, burnThinkGoto, "zombieFBurnSearch";
		AISTATE "zombieFBurnSearch", "+3", 3, 3600, null, null, aiMoveForward, burnThinkSearch, "none";
		AISTATE "zombieFBurnAttack", "+3", 4, 120, BurnSeqCallback, null, null, null, "zombieFBurnChase";
		AISTATE "innocentBurnChase", "+3", 4, 0, null, null, aiMoveForward, burnThinkChase, "none";
		AISTATE "innocentBurnGoto", "+3", 2, 3600, null, null, aiMoveForward, burnThinkGoto, "zombieFBurnSearch";
		AISTATE "innocentBurnSearch", "+3", 3, 3600, null, null, aiMoveForward, burnThinkSearch, "none";
		AISTATE "innocentBurnAttack", "+3", 4, 120, BurnSeqCallback, null, null, null, "zombieFBurnChase";
		AISTATE "beastBurnChase", "+3", 4, 0, null, null, aiMoveForward, burnThinkChase, "none";
		AISTATE "beastBurnGoto", "+3", 2, 3600, null, null, aiMoveForward, burnThinkGoto, "beastBurnSearch";
		AISTATE "beastBurnSearch", "+3", 3, 3600, null, null, aiMoveForward, burnThinkSearch, "beastBurnSearch";
		AISTATE "beastBurnAttack", "+3", 4, 120, BurnSeqCallback, null, null, null, "beastBurnChase";
		AISTATE "tinycalebBurnChase", "+3", 4, 0, null, null, aiMoveForward, burnThinkChase, "none";
		AISTATE "tinycalebBurnGoto", "+3", 2, 3600, null, null, aiMoveForward, burnThinkGoto, "tinycalebBurnSearch";
		AISTATE "tinycalebBurnSearch", "+3", 3, 3600, null, null, aiMoveForward, burnThinkSearch, "tinycalebBurnSearch";
		AISTATE "tinycalebBurnAttack", "+3", 4, 120, BurnSeqCallback, null, null, null, "tinycalebBurnChase";
		AISTATE "genDudeBurnIdle", "+3", 0, 0, null, null, null, aiThinkTarget, "none";
		AISTATE "genDudeBurnChase", "+3", 4, 0, null, null, aiMoveForward, burnThinkChase, "none";
		AISTATE "genDudeBurnGoto", "+3", 2, 3600, null, null, aiMoveForward, burnThinkGoto, "genDudeBurnSearch";
		AISTATE "genDudeBurnSearch", "+3", 3, 3600, null, null, aiMoveForward, burnThinkSearch, "genDudeBurnSearch";
		AISTATE "genDudeBurnAttack", "+3", 4, 120, BurnSeqCallback, null, null, null, "genDudeBurnChase";
		AISTATE "tinycalebIdle", "+0", 0, 0, null, null, null, aiThinkTarget, "none";
		AISTATE "tinycalebChase", "+6", 4, 0, null, null, aiMoveForward, calebThinkChase, "none";
		AISTATE "tinycalebDodge", "+6", 2, 90, null, null, aiMoveDodge, null, "tinycalebChase";
		AISTATE "tinycalebGoto", "+6", 2, 600, null, null, aiMoveForward, calebThinkGoto, "tinycalebIdle";
		AISTATE "tinycalebAttack", "+0", 4, 120, SeqAttackCallback, null, null, null, "tinycalebChase";
		AISTATE "tinycalebSearch", "+6", 3, 120, null, null, aiMoveForward, calebThinkSearch, "tinycalebIdle";
		AISTATE "tinycalebRecoil", "+5", 5, 0, null, null, null, null, "tinycalebDodge";
		AISTATE "tinycalebTeslaRecoil", "+4", 5, 0, null, null, null, null, "tinycalebDodge";
		AISTATE "tinycalebSwimIdle", "+10", 0, 0, null, null, null, aiThinkTarget, "none";
		AISTATE "tinycalebSwimChase", "+8", 4, 0, null, null, calebMoveSwimChase, calebThinkSwimChase, "none";
		AISTATE "tinycalebSwimDodge", "+8", 2, 90, null, null, aiMoveDodge, null, "tinycalebSwimChase";
		AISTATE "tinycalebSwimGoto", "+8", 2, 600, null, null, aiMoveForward, calebThinkSwimGoto, "tinycalebSwimIdle";
		AISTATE "tinycalebSwimSearch", "+8", 3, 120, null, null, aiMoveForward, calebThinkSearch, "tinycalebSwimIdle";
		AISTATE "tinycalebSwimAttack", "+10", 4, 0, SeqAttackCallback, null, null, null, "tinycalebSwimChase";
		AISTATE "tinycalebSwimRecoil", "+5", 5, 0, null, null, null, null, "tinycalebSwimDodge";
		AISTATE "tinycalebSwimUnused", "+8", -1, 120, null, null, calebSwimUnused, calebThinkSwimChase, "tinycalebSwimChase";
		AISTATE "tinycalebSwimMoveIn", "+8", -1, 0, null, null, calebSwimMoveIn, calebThinkSwimChase, "tinycalebSwimChase";
		AISTATE "tinycalebSwimTurn", "+8", -1, 120, null, null, aiMoveTurn, null, "tinycalebSwimChase";
		AISTATE "cerberusIdle", "+0", 0, 0, null, null, null, cerberusThinkTarget, "none";
		AISTATE "cerberusSearch", "+7", 3, 1800, null, null, aiMoveForward, cerberusThinkSearch, "cerberusIdle";
		AISTATE "cerberusChase", "+7", 4, 0, null, null, aiMoveForward, cerberusThinkChase, "none";
		AISTATE "cerberusRecoil", "+5", 5, 0, null, null, null, null, "cerberusSearch";
		AISTATE "cerberusTeslaRecoil", "+4", 5, 0, null, null, null, null, "cerberusSearch";
		AISTATE "cerberusGoto", "+7", 2, 600, null, null, aiMoveForward, cerberusThinkGoto, "cerberusIdle";
		AISTATE "cerberusBite", "+6", 4, 60, cerberusBiteSeqCallback, null, null, null, "cerberusChase";
		AISTATE "cerberusBurn", "+6", 4, 60, cerberusBurnSeqCallback, null, null, null, "cerberusChase";
		AISTATE "cerberus3Burn", "+6", 4, 60, cerberusBurnSeqCallback2, null, null, null, "cerberusChase";
		AISTATE "cerberus2Idle", "+0", 0, 0, null, null, null, cerberusThinkTarget, "none";
		AISTATE "cerberus2Search", "+7", 3, 1800, null, null, aiMoveForward, cerberusThinkSearch, "cerberus2Idle";
		AISTATE "cerberus2Chase", "+7", 4, 0, null, null, aiMoveForward, cerberusThinkChase, "none";
		AISTATE "cerberus2Recoil", "+5", 5, 0, null, null, null, null, "cerberus2Search";
		AISTATE "cerberus2Goto", "+7", 2, 600, null, null, aiMoveForward, cerberusThinkGoto, "cerberus2Idle";
		AISTATE "cerberus2Bite", "+6", 4, 60, cerberusBiteSeqCallback, null, null, null, "cerberus2Chase";
		AISTATE "cerberus2Burn", "+6", 4, 60, cerberusBurnSeqCallback, null, null, null, "cerberus2Chase";
		AISTATE "cerberus4Burn", "+6", 4, 60, cerberusBurnSeqCallback2, null, null, null, "cerberus2Chase";
		AISTATE "cerberusTurn1", "+7", -1, 120, null, null, aiMoveTurn, null, "cerberusChase";
		AISTATE "cerberusTurn2", "+7", -1, 120, null, null, aiMoveTurn, null, "cerberusChase";
		AISTATE "cultistIdle", "+0", 0, 0, null, null, null, aiThinkTarget, "none";
		AISTATE "cultistProneIdle", "+17", 0, 0, null, null, null, aiThinkTarget, "none";
		AISTATE "fanaticProneIdle", "+17", 0, 0, null, null, null, aiThinkTarget, "none";
		AISTATE "cultistProneIdle3", "+17", 0, 0, null, null, null, aiThinkTarget, "none";
		AISTATE "cultistChase", "+9", 4, 0, null, null, aiMoveForward, cultThinkChase, "none";
		AISTATE "fanaticChase", "+0", 4, 0, null, null, aiMoveTurn, cultThinkChase, "none";
		AISTATE "cultistDodge", "+9", 2, 90, null, null, aiMoveDodge, null, "cultistChase";
		AISTATE "cultistGoto", "+9", 2, 600, null, null, aiMoveForward, cultThinkGoto, "cultistIdle";
		AISTATE "cultistProneChase", "+14", 4, 0, null, null, aiMoveForward, cultThinkChase, "none";
		AISTATE "cultistProneDodge", "+14", 2, 90, null, null, aiMoveDodge, null, "cultistProneChase";
		AISTATE "cultistTThrow", "+7", 4, 120, cultThrowSeqCallback, null, null, null, "cultistTFire";
		AISTATE "cultistSThrow", "+7", 4, 120, cultThrowSeqCallback, null, null, null, "cultistSFire";
		AISTATE "cultistTsThrow", "+7", 4, 120, cultThrowSeqCallback, null, null, null, "cultistTsFire";
		AISTATE "cultistDThrow", "+7", 4, 120, cultThrowSeqCallback, null, null, null, "cultistChase";
		AISTATE "cultistDThrow2", "+7", 4, 120, cultThrowSeqCallback2, null, null, null, "cultistChase";
		AISTATE "cultistDThrow3C", "+7", 4, 120, cultThrowSeqCallback3, null, null, null, "cultistIdle";
		AISTATE "cultistDThrow3B", "+7", 4, 120, cultThrowSeqCallback3, null, null, cultThinkSearch, "cultistDThrow3C";
		AISTATE "cultistDThrow3A", "+7", 4, 120, cultThrowSeqCallback3, null, null, cultThinkSearch, "cultistDThrow3B";
		AISTATE "cultistDThrow4", "+7", 4, 120, cultThrowSeqCallback3, null, null, cultThinkSearch, "cultistDThrow4";
		AISTATE "cultistSearch", "+9", 3, 1800, null, null, aiMoveForward, cultThinkSearch, "cultistIdle";
		AISTATE "cultistSFire", "+6", 4, 60, ShotSeqCallback, null, null, null, "cultistChase";
		AISTATE "cultistTFire", "+6", 4, 0, TommySeqCallback, null, aiMoveTurn, cultThinkChase, "cultistTFire";
		AISTATE "cultistTsFire", "+6", 4, 0, TeslaSeqCallback, null, aiMoveTurn, cultThinkChase, "cultistChase";
		AISTATE "cultistSProneFire", "+8", 4, 60, ShotSeqCallback, null, null, null, "cultistProneChase";
		AISTATE "cultistTProneFire", "+8", 4, 0, TommySeqCallback, null, aiMoveTurn, cultThinkChase, "cultistTProneFire";
		AISTATE "cultistTsProneFire", "+8", 4, 0, TeslaSeqCallback, null, aiMoveTurn, null, "cultistTsProneFire";
		AISTATE "cultistTsProneFireFixed", "+0", 8, 0, TeslaSeqCallback, null, aiMoveTurn, null, "cultistTsProneFireFixed";
		AISTATE "cultistRecoil", "+5", 5, 0, null, null, null, null, "cultistDodge";
		AISTATE "cultistProneRecoil", "+5", 5, 0, null, null, null, null, "cultistProneDodge";
		AISTATE "cultistTeslaRecoil", "+4", 5, 0, null, null, null, null, "cultistDodge";
		AISTATE "cultistSwimIdle", "+13", 0, 0, null, null, null, aiThinkTarget, "none";
		AISTATE "cultistSwimChase", "+13", 4, 0, null, null, aiMoveForward, cultThinkChase, "none";
		AISTATE "cultistSwimDodge", "+13", 2, 90, null, null, aiMoveDodge, null, "cultistSwimChase";
		AISTATE "cultistSwimGoto", "+13", 2, 600, null, null, aiMoveForward, cultThinkGoto, "cultistSwimIdle";
		AISTATE "cultistSwimSearch", "+13", 3, 1800, null, null, aiMoveForward, cultThinkSearch, "cultistSwimIdle";
		AISTATE "cultistSSwimFire", "+8", 4, 60, ShotSeqCallback, null, null, null, "cultistSwimChase";
		AISTATE "cultistTSwimFire", "+8", 4, 0, TommySeqCallback, null, aiMoveTurn, cultThinkChase, "cultistTSwimFire";
		AISTATE "cultistTsSwimFire", "+8", 4, 0, TeslaSeqCallback, null, aiMoveTurn, cultThinkChase, "cultistTsSwimFire";
		AISTATE "cultistSwimRecoil", "+5", 5, 0, null, null, null, null, "cultistSwimDodge";
		AISTATE "gargoyleFIdle", "+0", 0, 0, null, null, null, gargThinkTarget, "none";
		AISTATE "gargoyleStatueIdle", "+0", 0, 0, null, null, null, null, "none";
		AISTATE "gargoyleFChase", "+0", 4, 0, null, null, gargMoveForward, gargThinkChase, "gargoyleFIdle";
		AISTATE "gargoyleFGoto", "+0", 2, 600, null, null, gargMoveForward, gargThinkGoto, "gargoyleFIdle";
		AISTATE "gargoyleFSlash", "+6", 4, 120, SlashFSeqCallback, null, null, null, "gargoyleFChase";
		AISTATE "gargoyleFThrow", "+6", 4, 120, ThrowFSeqCallback, null, null, null, "gargoyleFChase";
		AISTATE "gargoyleSThrow", "+6", 4, 120, ThrowSSeqCallback, null, gargMoveForward, null, "gargoyleFChase";
		AISTATE "gargoyleSBlast", "+7", 4, 60, BlastSSeqCallback, null, gargMoveSlow, null, "gargoyleFChase";
		AISTATE "gargoyleFRecoil", "+5", 5, 0, null, null, null, null, "gargoyleFChase";
		AISTATE "gargoyleFSearch", "+0", 3, 120, null, null, gargMoveForward, gargThinkSearch, "gargoyleFIdle";
		AISTATE "gargoyleFMorph", "+6", -1, 0, null, null, null, null, "gargoyleFMorph2";
		AISTATE "gargoyleSMorph", "+6", -1, 0, null, null, null, null, "gargoyleSMorph2";
		AISTATE "gargoyleSwoop", "+0", -1, 120, null, null, gargMoveSwoop, gargThinkChase, "gargoyleFChase";
		AISTATE "gargoyleFly", "+0", 2, 120, null, null, gargMoveFly, gargThinkChase, "gargoyleFChase";
		AISTATE "gargoyleTurn", "+0", 2, 120, null, null, aiMoveTurn, null, "gargoyleFChase";
		AISTATE "gargoyleDodgeUp", "+0", 2, 60, null, null, gargMoveDodgeUp, null, "gargoyleFChase";
		AISTATE "gargoyleFDodgeUpRight", "+0", 2, 90, null, null, gargMoveDodgeUp, null, "gargoyleFChase";
		AISTATE "gargoyleFDodgeUpLeft", "+0", 2, 90, null, null, gargMoveDodgeUp, null, "gargoyleFChase";
		AISTATE "gargoyleDodgeDown", "+0", 2, 120, null, null, gargMoveDodgeDown, null, "gargoyleFChase";
		AISTATE "gargoyleFDodgeDownRight", "+0", 2, 90, null, null, gargMoveDodgeDown, null, "gargoyleFChase";
		AISTATE "gargoyleFDodgeDownLeft", "+0", 2, 90, null, null, gargMoveDodgeDown, null, "gargoyleFChase";
		AISTATE "statueFBreakSEQ", "+5", -1, 0, null, entryFStatue, null, playStatueBreakSnd, "gargoyleFMorph2";
		AISTATE "statueSBreakSEQ", "+5", -1, 0, null, entrySStatue, null, playStatueBreakSnd, "gargoyleSMorph2";
		AISTATE "ghostIdle", "+0", 0, 0, null, null, null, ghostThinkTarget, "none";
		AISTATE "ghostChase", "+0", 4, 0, null, null, ghostMoveForward, ghostThinkChase, "ghostIdle";
		AISTATE "ghostGoto", "+0", 2, 600, null, null, ghostMoveForward, ghostThinkGoto, "ghostIdle";
		AISTATE "ghostSlash", "+6", 4, 120, ghostSlashSeqCallback, null, null, null, "ghostChase";
		AISTATE "ghostThrow", "+6", 4, 120, ghostThrowSeqCallback, null, null, null, "ghostChase";
		AISTATE "ghostBlast", "+6", 4, 120, ghostBlastSeqCallback, null, ghostMoveSlow, null, "ghostChase";
		AISTATE "ghostRecoil", "+5", 5, 0, null, null, null, null, "ghostChase";
		AISTATE "ghostTeslaRecoil", "+4", 5, 0, null, null, null, null, "ghostChase";
		AISTATE "ghostSearch", "+0", 3, 120, null, null, ghostMoveForward, ghostThinkSearch, "ghostIdle";
		AISTATE "ghostSwoop", "+0", -1, 120, null, null, ghostMoveSwoop, ghostThinkChase, "ghostChase";
		AISTATE "ghostFly", "+0", 2, 0, null, null, ghostMoveFly, ghostThinkChase, "ghostChase";
		AISTATE "ghostTurn", "+0", 2, 120, null, null, aiMoveTurn, null, "ghostChase";
		AISTATE "ghostDodgeUp", "+0", 2, 60, null, null, ghostMoveDodgeUp, null, "ghostChase";
		AISTATE "ghostDodgeUpRight", "+0", 2, 90, null, null, ghostMoveDodgeUp, null, "ghostChase";
		AISTATE "ghostDodgeUpLeft", "+0", 2, 90, null, null, ghostMoveDodgeUp, null, "ghostChase";
		AISTATE "ghostDodgeDown", "+0", 2, 120, null, null, ghostMoveDodgeDown, null, "ghostChase";
		AISTATE "ghostDodgeDownRight", "+0", 2, 90, null, null, ghostMoveDodgeDown, null, "ghostChase";
		AISTATE "ghostDodgeDownLeft", "+0", 2, 90, null, null, ghostMoveDodgeDown, null, "ghostChase";
		AISTATE "gillBeastIdle", "+0", 0, 0, null, null, null, aiThinkTarget, "none";
		AISTATE "gillBeastChase", "+9", 4, 0, null, null, aiMoveForward, gillThinkChase, "none";
		AISTATE "gillBeastDodge", "+9", 2, 90, null, null, aiMoveDodge, null, "gillBeastChase";
		AISTATE "gillBeastGoto", "+9", 2, 600, null, null, aiMoveForward, gillThinkGoto, "gillBeastIdle";
		AISTATE "gillBeastBite", "+6", 4, 120, GillBiteSeqCallback, null, null, null, "gillBeastChase";
		AISTATE "gillBeastSearch", "+9", 2, 120, null, null, aiMoveForward, gillThinkSearch, "gillBeastIdle";
		AISTATE "gillBeastRecoil", "+5", 5, 0, null, null, null, null, "gillBeastDodge";
		AISTATE "gillBeastSwimIdle", "+10", 0, 0, null, null, null, aiThinkTarget, "none";
		AISTATE "gillBeastSwimChase", "+10", 4, 0, null, null, gillMoveSwimChase, gillThinkSwimChase, "none";
		AISTATE "gillBeastSwimDodge", "+10", 2, 90, null, null, aiMoveDodge, null, "gillBeastSwimChase";
		AISTATE "gillBeastSwimGoto", "+10", 2, 600, null, null, aiMoveForward, gillThinkSwimGoto, "gillBeastSwimIdle";
		AISTATE "gillBeastSwimSearch", "+10", 3, 120, null, null, aiMoveForward, gillThinkSearch, "gillBeastSwimIdle";
		AISTATE "gillBeastSwimBite", "+7", 4, 0, GillBiteSeqCallback, null, null, gillThinkSwimChase, "gillBeastSwimChase";
		AISTATE "gillBeastSwimRecoil", "+5", 5, 0, null, null, null, null, "gillBeastSwimDodge";
		AISTATE "gillBeastSwimUnused", "+10", -1, 120, null, null, gillMoveSwimUnused, gillThinkSwimChase, "gillBeastSwimChase";
		AISTATE "gillBeastSwimMoveIn", "+10", -1, 0, null, null, gillSwimMoveIn, gillThinkSwimChase, "gillBeastSwimChase";
		AISTATE "gillBeastSwimTurn", "+10", -1, 120, null, null, null, aiMoveTurn, "gillBeastSwimChase";
		AISTATE "handIdle", "+0", 0, 0, null, null, null, aiThinkTarget, "none";
		AISTATE "handChoke", "+0", -1, 0, null, null, null, null, "none";
		AISTATE "handSearch", "+6", 2, 600, null, null, aiMoveForward, handThinkSearch, "handIdle";
		AISTATE "handChase", "+6", 4, 0, null, null, aiMoveForward, handThinkChase, "none";
		AISTATE "handRecoil", "+5", 5, 0, null, null, null, null, "handSearch";
		AISTATE "handGoto", "+6", 2, 1800, null, null, aiMoveForward, handThinkGoto, "handIdle";
		AISTATE "handJump", "+7", 4, 120, HandJumpSeqCallback, null, null, null, "handChase";
		AISTATE "houndIdle", "+0", 0, 0, null, null, null, aiThinkTarget, "none";
		AISTATE "houndSearch", "+8", 2, 1800, null, null, aiMoveForward, houndThinkSearch, "houndIdle";
		AISTATE "houndChase", "+8", 4, 0, null, null, aiMoveForward, houndThinkChase, "none";
		AISTATE "houndRecoil", "+5", 5, 0, null, null, null, null, "houndSearch";
		AISTATE "houndTeslaRecoil", "+4", 5, 0, null, null, null, null, "houndSearch";
		AISTATE "houndGoto", "+8", 2, 600, null, null, aiMoveForward, houndThinkGoto, "houndIdle";
		AISTATE "houndBite", "+6", 4, 60, houndBiteSeqCallback, null, null, null, "houndChase";
		AISTATE "houndBurn", "+7", 4, 60, houndBurnSeqCallback, null, null, null, "houndChase";
		AISTATE "innocentIdle", "+0", 0, 0, null, null, null, aiThinkTarget, "none";
		AISTATE "innocentSearch", "+6", 3, 1800, null, null, aiMoveForward, innocThinkSearch, "innocentIdle";
		AISTATE "innocentChase", "+6", 4, 0, null, null, aiMoveForward, innocThinkChase, "none";
		AISTATE "innocentRecoil", "+5", 5, 0, null, null, null, null, "innocentChase";
		AISTATE "innocentTeslaRecoil", "+4", 5, 0, null, null, null, null, "innocentChase";
		AISTATE "innocentGoto", "+6", 2, 600, null, null, aiMoveForward, innocThinkGoto, "innocentIdle";
		AISTATE "podIdle", "+0", 0, 0, null, null, null, aiThinkTarget, "none";
		AISTATE "podMove", "+7", 2, 3600, null, null, aiMoveTurn, aiPodMove, "podSearch";
		AISTATE "podSearch", "+0", 3, 3600, null, null, aiMoveTurn, aiPodSearch, "podSearch";
		AISTATE "podStartChase", "+8", 4, 600, podAttack, null, null, null, "podChase";
		AISTATE "podRecoil", "+5", 5, 0, null, null, null, null, "podChase";
		AISTATE "podChase", "+6", 4, 0, null, null, aiMoveTurn, aiPodChase, "none";
		AISTATE "tentacleIdle", "+0", 0, 0, null, null, null, aiThinkTarget, "none";
		AISTATE "tentacle13A6A8", "+7", -1, 0, podPlaySound1, null, null, null, "tentacle13A6C4";
		AISTATE "tentacle13A6E0", "+8", -1, 0, podPlaySound2, null, null, null, "tentacle13A6FC";
		AISTATE "tentacleMove", "+8", -1, 3600, null, null, aiMoveTurn, aiPodMove, "tentacleSearch";
		AISTATE "tentacleSearch", "+0", -1, 3600, null, null, aiMoveTurn, aiPodSearch, "none";
		AISTATE "tentacleStartChase", "+6", -1, 120, podExplode, null, null, null, "tentacleChase";
		AISTATE "tentacleRecoil", "+5", 5, 0, null, null, null, null, "tentacleChase";
		AISTATE "tentacleChase", "+6", 4, 0, null, null, aiMoveTurn, aiPodChase, "none";
		AISTATE "ratIdle", "+0", 0, 0, null, null, null, aiThinkTarget, "none";
		AISTATE "ratSearch", "+7", 3, 1800, null, null, aiMoveForward, ratThinkSearch, "ratIdle";
		AISTATE "ratChase", "+7", 4, 0, null, null, aiMoveForward, ratThinkChase, "none";
		AISTATE "ratDodge", "+7", 2, 0, null, null, null, null, "ratChase";
		AISTATE "ratRecoil", "+7", 5, 0, null, null, null, null, "ratDodge";
		AISTATE "ratGoto", "+7", 2, 600, null, null, aiMoveForward, ratThinkGoto, "ratIdle";
		AISTATE "ratBite", "+6", 4, 120, ratBiteSeqCallback, null, null, null, "ratChase";
		AISTATE "spidIdle", "+0", 0, 0, null, null, null, aiThinkTarget, "none";
		AISTATE "spidChase", "+7", 4, 0, null, null, aiMoveForward, spidThinkChase, "none";
		AISTATE "spidDodge", "+7", 2, 90, null, null, aiMoveDodge, null, "spidChase";
		AISTATE "spidGoto", "+7", 2, 600, null, null, aiMoveForward, spidThinkGoto, "spidIdle";
		AISTATE "spidSearch", "+7", 3, 1800, null, null, aiMoveForward, spidThinkSearch, "spidIdle";
		AISTATE "spidBite", "+6", 4, 60, SpidBiteSeqCallback, null, null, null, "spidChase";
		AISTATE "spidJump", "+8", 4, 60, SpidJumpSeqCallback, null, aiMoveForward, null, "spidChase";
		AISTATE "spidBirth", "+0", -1, 60, SpidBirthSeqCallback, null, null, null, "spidIdle";
		AISTATE "tchernobogIdle", "+0", 0, 0, null, null, null, tchernobogThinkTarget, "none";
		AISTATE "tchernobogSearch", "+8", 3, 1800, null, null, aiMoveForward, tchernobogThinkSearch, "tchernobogIdle";
		AISTATE "tchernobogChase", "+8", 4, 0, null, null, aiMoveForward, tchernobogThinkChase, "none";
		AISTATE "tchernobogRecoil", "+5", 5, 0, null, null, null, null, "tchernobogSearch";
		AISTATE "tchernobogGoto", "+8", 2, 600, null, null, aiMoveForward, tchernobogThinkGoto, "tchernobogIdle";
		AISTATE "tchernobogBurn1", "+6", 2, 60, tchernobogBurnSeqCallback, null, null, null, "tchernobogChase";
		AISTATE "tchernobogBurn2", "+6", 4, 60, tchernobogBurnSeqCallback2, null, null, null, "tchernobogChase";
		AISTATE "tchernobogFireAtk", "+7", 4, 60, tchernobogFire, null, null, null, "tchernobogChase";
		AISTATE "tchernobogTurn", "+8", 4, 60, null, null, aiMoveTurn, null, "tchernobogChase";
		AISTATE "zombieAIdle", "+0", 0, 0, null, entryAIdle, null, aiThinkTarget, "none";
		AISTATE "zombieAChase", "+8", 4, 0, null, null, aiMoveForward, zombaThinkChase, "none";
		AISTATE "zombieAPonder", "+0", -1, 0, null, null, aiMoveTurn, zombaThinkPonder, "none";
		AISTATE "zombieAGoto", "+8", 2, 1800, null, null, aiMoveForward, zombaThinkGoto, "zombieAIdle";
		AISTATE "zombieAHack", "+6", 4, 80, HackSeqCallback, null, null, null, "zombieAPonder";
		AISTATE "zombieASearch", "+8", 3, 1800, null, null, aiMoveForward, zombaThinkSearch, "zombieAIdle";
		AISTATE "zombieARecoil", "+5", 5, 0, null, null, null, null, "zombieAPonder";
		AISTATE "zombieATeslaRecoil", "+4", 5, 0, null, null, null, null, "zombieAPonder";
		AISTATE "zombieARecoil2", "+1", 5, 360, null, null, null, null, "zombieAStand";
		AISTATE "zombieAStand", "+11", 2, 0, StandSeqCallback, null, null, null, "zombieAPonder";
		AISTATE "zombieEIdle", "+12", 0, 0, null, null, null, aiThinkTarget, "none";
		AISTATE "zombieEUp2", "+0", 2, 1, null, entryEZombie, null, null, "zombieASearch";
		AISTATE "zombieEUp", "+9", 2, 180, null, entryEStand, null, null, "zombieEUp2";
		AISTATE "zombie2Idle", "+0", 0, 0, null, entryAIdle, null, myThinkTarget, "none";
		AISTATE "zombie2Search", "+8", 3, 1800, null, null, null, myThinkSearch, "zombie2Idle";
		AISTATE "zombieSIdle", "+10", 0, 0, null, null, null, aiThinkTarget, "none";
		AISTATE "zombieEStand", "+11", -1, 0, StandSeqCallback, entryEZombie, null, null, "zombieAPonder";
		AISTATE "zombieFIdle", "+0", 0, 0, null, null, null, aiThinkTarget, "none";
		AISTATE "zombieFChase", "+8", 4, 0, null, null, aiMoveForward, zombfThinkChase, "none";
		AISTATE "zombieFGoto", "+8", 2, 600, null, null, aiMoveForward, zombfThinkGoto, "zombieFIdle";
		AISTATE "zombieFDodge", "+8", 2, 0, null, null, aiMoveDodge, zombfThinkChase, "zombieFChase";
		AISTATE "zombieFHack", "+6", 4, 120, zombfHackSeqCallback, null, null, null, "zombieFChase";
		AISTATE "zombieFPuke", "+9", 4, 120, PukeSeqCallback, null, null, null, "zombieFChase";
		AISTATE "zombieFThrow", "+6", 4, 120, ThrowSeqCallback, null, null, null, "zombieFChase";
		AISTATE "zombieFSearch", "+8", 3, 1800, null, null, aiMoveForward, zombfThinkSearch, "zombieFIdle";
		AISTATE "zombieFRecoil", "+5", 5, 0, null, null, null, null, "zombieFChase";
		AISTATE "zombieFTeslaRecoil", "+4", 5, 0, null, null, null, null, "zombieFChase";
	}
	override int getRespawnTime()
	{
		if (!self.hasX) return -1;
		if (self.xspr.respawn == 2 || (self.xspr.respawn != 1 && gGameOptions.nMonsterSettings == Blood.MONSTERSETTINGS_2))
			return gGameOptions.nMonsterRespawnTime;
		return -1;
	}
}

class BloodPlayerBase : BloodDudeBase
{
	override int getRespawnTime()
	{
		return -1;	// no respawn for players.
	}
}

class BloodDudeCultistTommy : BloodDudeBase {}
class BloodDudeCultistShotgun : BloodDudeBase {}
class BloodDudeZombieAxeNormal : BloodDudeBase {}
class BloodDudeZombieButcher : BloodDudeBase {}
class BloodDudeZombieAxeBuried : BloodDudeBase {}
class BloodDudeGargoyleFlesh : BloodDudeBase {}
class BloodDudeGargoyleStone : BloodDudeBase {}
class BloodDudeGargoyleStatueFlesh : BloodDudeBase {}
class BloodDudeGargoyleStatueStone : BloodDudeBase {}
class BloodDudePhantasm : BloodDudeBase {}
class BloodDudeHellHound : BloodDudeBase {}
class BloodDudeHand : BloodDudeBase {}
class BloodDudeSpiderBrown : BloodDudeBase {}
class BloodDudeSpiderRed : BloodDudeBase {}
class BloodDudeSpiderBlack : BloodDudeBase {}
class BloodDudeSpiderMother : BloodDudeBase {}
class BloodDudeGillBeast : BloodDudeBase {}
class BloodDudeBoneEel : BloodDudeBase {}
class BloodDudeBat : BloodDudeBase {}
class BloodDudeRat : BloodDudeBase {}
class BloodDudePodGreen : BloodDudeBase {}
class BloodDudeTentacleGreen : BloodDudeBase {}
class BloodDudePodFire : BloodDudeBase {}
class BloodDudeTentacleFire : BloodDudeBase {}
class BloodDudePodMother : BloodDudeBase {}
class BloodDudeTentacleMother : BloodDudeBase {}
class BloodDudeCerberusTwoHead : BloodDudeBase {}
class BloodDudeCerberusOneHead : BloodDudeBase {}
class BloodDudeTchernobog : BloodDudeBase {}
class BloodDudeCultistTommyProne : BloodDudeBase {}
class BloodDudePlayer1 : BloodPlayerBase {}
class BloodDudePlayer2 : BloodPlayerBase {}
class BloodDudePlayer3 : BloodPlayerBase {}
class BloodDudePlayer4 : BloodPlayerBase {}
class BloodDudePlayer5 : BloodPlayerBase {}
class BloodDudePlayer6 : BloodPlayerBase {}
class BloodDudePlayer7 : BloodPlayerBase {}
class BloodDudePlayer8 : BloodPlayerBase {}
class BloodDudeBurningInnocent : BloodDudeBase {}
class BloodDudeBurningCultist : BloodDudeBase {}
class BloodDudeBurningZombieAxe : BloodDudeBase {}
class BloodDudeBurningZombieButcher : BloodDudeBase {}
class BloodDudeCultistReserved : BloodDudeBase {} // unused
class BloodDudeZombieAxeLaying : BloodDudeBase {}
class BloodDudeInnocent : BloodDudeBase {}
class BloodDudeCultistShotgunProne : BloodDudeBase {}
class BloodDudeCultistTesla : BloodDudeBase {}
class BloodDudeCultistTNT : BloodDudeBase {}
class BloodDudeCultistBeast : BloodDudeBase {}
class BloodDudeTinyCaleb : BloodDudeBase {}
class BloodDudeBeast : BloodDudeBase {}
class BloodDudeBurningTinyCaleb : BloodDudeBase {}
class BloodDudeBurningBeast : BloodDudeBase {}
