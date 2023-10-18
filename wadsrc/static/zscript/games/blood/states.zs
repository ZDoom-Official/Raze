extend class BloodActor
{
	enum AIStateTypes
	{
		kAiStateOther = -1,
		kAiStateIdle = 0,
		kAiStateGenIdle = 1,
		kAiStateMove = 2,
		kAiStateSearch = 3,
		kAiStateChase = 4,
		kAiStateRecoil = 5,
		kAiStateAttack = 6,

		kAiStatePatrolBase,
		kAiStateKnockout = kAiStatePatrolBase,
		kAiStateIdleSleep,
		kAiStatePatrolWaitL,
		kAiStatePatrolWaitC,
		kAiStatePatrolWaitW,
		kAiStatePatrolMoveL,
		kAiStatePatrolMoveC,
		kAiStatePatrolMoveW,
		kAiStatePatrolTurnL,
		kAiStatePatrolTurnC,
		kAiStatePatrolTurnW,
		kAiStatePatrolMax
	}

	default
	{
		AIState "genIdle", "+0", kAiStateGenIdle, 0, null, null, null, null, "none";
		AIState "genRecoil", "+5", kAiStateRecoil, 20, null, null, null, null, "genIdle";
		AIState "batIdle", "+0", kAiStateIdle, 0, null, null, null, batThinkTarget, "none";
		AIState "batFlyIdle", "+6", kAiStateIdle, 0, null, null, null, batThinkTarget, "none";
		AIState "batChase", "+6", kAiStateChase, 0, null, null, batMoveForward, batThinkChase, "batFlyIdle";
		AIState "batPonder", "+6", kAiStateOther, 0, null, null, null, batThinkPonder, "none";
		AIState "batGoto", "+6", kAiStateMove, 600, null, null, batMoveForward, batThinkGoto, "batFlyIdle";
		AIState "batBite", "+7", kAiStateChase, 60, batBiteSeqCallback, null, null, null, "batPonder";
		AIState "batRecoil", "+5", kAiStateRecoil, 0, null, null, null, null, "batChase";
		AIState "batSearch", "+6", kAiStateSearch, 120, null, null, batMoveForward, batThinkSearch, "batFlyIdle";
		AIState "batSwoop", "+6", kAiStateOther, 60, null, null, batMoveSwoop, batThinkChase, "batChase";
		AIState "batFly", "+6", kAiStateMove, 0, null, null, batMoveFly, batThinkChase, "batChase";
		AIState "batTurn", "+6", kAiStateMove, 60, null, null, aiMoveTurn, null, "batChase";
		AIState "batHide", "+6", kAiStateOther, 0, null, null, batMoveToCeil, batMoveForward, "none";
		AIState "batDodgeUp", "+6", kAiStateMove, 120, null, null, batMoveDodgeUp, null, "batChase";
		AIState "batDodgeUpRight", "+6", kAiStateMove, 90, null, null, batMoveDodgeUp, null, "batChase";
		AIState "batDodgeUpLeft", "+6", kAiStateMove, 90, null, null, batMoveDodgeUp, null, "batChase";
		AIState "batDodgeDown", "+6", kAiStateMove, 120, null, null, batMoveDodgeDown, null, "batChase";
		AIState "batDodgeDownRight", "+6", kAiStateMove, 90, null, null, batMoveDodgeDown, null, "batChase";
		AIState "batDodgeDownLeft", "+6", kAiStateMove, 90, null, null, batMoveDodgeDown, null, "batChase";
		AIState "beastIdle", "+0", kAiStateIdle, 0, null, null, null, aiThinkTarget, "none";
		AIState "beastChase", "+8", kAiStateChase, 0, null, null, beastMoveForward, beastThinkChase, "none";
		AIState "beastDodge", "+8", kAiStateMove, 60, null, null, aiMoveDodge, null, "beastChase";
		AIState "beastGoto", "+8", kAiStateMove, 600, null, null, beastMoveForward, beastThinkGoto, "beastIdle";
		AIState "beastSlash", "+6", kAiStateChase, 120, SlashSeqCallback, null, null, null, "beastChase";
		AIState "beastStomp", "+7", kAiStateChase, 120, StompSeqCallback, null, null, null, "beastChase";
		AIState "beastSearch", "+8", kAiStateSearch, 120, null, null, beastMoveForward, beastThinkSearch, "beastIdle";
		AIState "beastRecoil", "+5", kAiStateRecoil, 0, null, null, null, null, "beastDodge";
		AIState "beastTeslaRecoil", "+4", kAiStateRecoil, 0, null, null, null, null, "beastDodge";
		AIState "beastSwimIdle", "+9", kAiStateIdle, 0, null, null, null, aiThinkTarget, "none";
		AIState "beastSwimChase", "+9", kAiStateChase, 0, null, null, beastMoveSwim, beastThinkSwimChase, "none";
		AIState "beastSwimDodge", "+9", kAiStateMove, 90, null, null, aiMoveDodge, null, "beastSwimChase";
		AIState "beastSwimGoto", "+9", kAiStateMove, 600, null, null, beastMoveForward, beastThinkSwimGoto, "beastSwimIdle";
		AIState "beastSwimSearch", "+9", kAiStateSearch, 120, null, null, beastMoveForward, beastThinkSearch, "beastSwimIdle";
		AIState "beastSwimSlash", "+9", kAiStateChase, 0, SlashSeqCallback, null, null, beastThinkSwimChase, "beastSwimChase";
		AIState "beastSwimRecoil", "+5", kAiStateRecoil, 0, null, null, null, null, "beastSwimDodge";
		AIState "beastMorphToBeast", "+-1", kAiStateOther, 0, null, MorphToBeast, null, null, "beastIdle";
		AIState "beastMorphFromCultist", "+2576", kAiStateOther, 0, null, null, null, null, "beastMorphToBeast";
		AIState "beastMoveSwimChaseAlt", "+9", kAiStateOther, 120, null, null, beastMoveSwimAlt, beastThinkSwimChase, "beastSwimChase";
		AIState "beastSwimAttack", "+9", kAiStateOther, 0, null, null, beastMoveIn, beastThinkSwimChase, "beastSwimChase";
		AIState "beastSwimTurn", "+9", kAiStateOther, 120, null, null, aiMoveTurn, null, "beastSwimChase";
		AIState "eelIdle", "+0", kAiStateIdle, 0, null, null, null, eelThinkTarget, "none";
		AIState "eelFlyIdle", "+0", kAiStateIdle, 0, null, null, null, eelThinkTarget, "none";
		AIState "eelChase", "+0", kAiStateChase, 0, null, null, eelMoveForward, eelThinkChase, "eelIdle";
		AIState "eelPonder", "+0", kAiStateOther, 0, null, null, null, eelThinkPonder, "none";
		AIState "eelGoto", "+0", kAiStateMove, 600, null, null, null, eelThinkGoto, "eelIdle";
		AIState "eelBite", "+7", kAiStateChase, 60, eelBiteSeqCallback, null, null, null, "eelChase";
		AIState "eelRecoil", "+5", kAiStateRecoil, 0, null, null, null, null, "eelChase";
		AIState "eelSearch", "+0", kAiStateSearch, 120, null, null, eelMoveForward, eelThinkSearch, "eelIdle";
		AIState "eelSwoop", "+0", kAiStateOther, 60, null, null, eelMoveSwoop, eelThinkChase, "eelChase";
		AIState "eelFly", "+0", kAiStateMove, 0, null, null, eelMoveAscend, eelThinkChase, "eelChase";
		AIState "eelTurn", "+0", kAiStateMove, 60, null, null, aiMoveTurn, null, "eelChase";
		AIState "eelHide", "+0", kAiStateOther, 0, null, null, eelMoveToCeil, eelMoveForward, "none";
		AIState "eelDodgeUp", "+0", kAiStateMove, 120, null, null, eelMoveDodgeUp, null, "eelChase";
		AIState "eelDodgeUpRight", "+0", kAiStateMove, 90, null, null, eelMoveDodgeUp, null, "eelChase";
		AIState "eelDodgeUpLeft", "+0", kAiStateMove, 90, null, null, eelMoveDodgeUp, null, "eelChase";
		AIState "eelDodgeDown", "+0", kAiStateMove, 120, null, null, eelMoveDodgeDown, null, "eelChase";
		AIState "eelDodgeDownRight", "+0", kAiStateMove, 90, null, null, eelMoveDodgeDown, null, "eelChase";
		AIState "eelDodgeDownLeft", "+0", kAiStateMove, 90, null, null, eelMoveDodgeDown, null, "eelChase";
		AIState "cultistBurnIdle", "+3", kAiStateIdle, 0, null, null, null, aiThinkTarget, "none";
		AIState "cultistBurnChase", "+3", kAiStateChase, 0, null, null, aiMoveForward, burnThinkChase, "none";
		AIState "cultistBurnGoto", "+3", kAiStateMove, 3600, null, null, aiMoveForward, burnThinkGoto, "cultistBurnSearch";
		AIState "cultistBurnSearch", "+3", kAiStateSearch, 3600, null, null, aiMoveForward, burnThinkSearch, "cultistBurnSearch";
		AIState "cultistBurnAttack", "+3", kAiStateChase, 120, BurnSeqCallback, null, null, null, "cultistBurnChase";
		AIState "zombieABurnChase", "+3", kAiStateChase, 0, null, null, aiMoveForward, burnThinkChase, "none";
		AIState "zombieABurnGoto", "+3", kAiStateMove, 3600, null, null, aiMoveForward, burnThinkGoto, "zombieABurnSearch";
		AIState "zombieABurnSearch", "+3", kAiStateSearch, 3600, null, null, aiMoveForward, burnThinkSearch, "none";
		AIState "zombieABurnAttack", "+3", kAiStateChase, 120, BurnSeqCallback, null, null, null, "zombieABurnChase";
		AIState "zombieFBurnChase", "+3", kAiStateChase, 0, null, null, aiMoveForward, burnThinkChase, "none";
		AIState "zombieFBurnGoto", "+3", kAiStateMove, 3600, null, null, aiMoveForward, burnThinkGoto, "zombieFBurnSearch";
		AIState "zombieFBurnSearch", "+3", kAiStateSearch, 3600, null, null, aiMoveForward, burnThinkSearch, "none";
		AIState "zombieFBurnAttack", "+3", kAiStateChase, 120, BurnSeqCallback, null, null, null, "zombieFBurnChase";
		AIState "innocentBurnChase", "+3", kAiStateChase, 0, null, null, aiMoveForward, burnThinkChase, "none";
		AIState "innocentBurnGoto", "+3", kAiStateMove, 3600, null, null, aiMoveForward, burnThinkGoto, "zombieFBurnSearch";
		AIState "innocentBurnSearch", "+3", kAiStateSearch, 3600, null, null, aiMoveForward, burnThinkSearch, "none";
		AIState "innocentBurnAttack", "+3", kAiStateChase, 120, BurnSeqCallback, null, null, null, "zombieFBurnChase";
		AIState "beastBurnChase", "+3", kAiStateChase, 0, null, null, aiMoveForward, burnThinkChase, "none";
		AIState "beastBurnGoto", "+3", kAiStateMove, 3600, null, null, aiMoveForward, burnThinkGoto, "beastBurnSearch";
		AIState "beastBurnSearch", "+3", kAiStateSearch, 3600, null, null, aiMoveForward, burnThinkSearch, "beastBurnSearch";
		AIState "beastBurnAttack", "+3", kAiStateChase, 120, BurnSeqCallback, null, null, null, "beastBurnChase";
		AIState "tinycalebBurnChase", "+3", kAiStateChase, 0, null, null, aiMoveForward, burnThinkChase, "none";
		AIState "tinycalebBurnGoto", "+3", kAiStateMove, 3600, null, null, aiMoveForward, burnThinkGoto, "tinycalebBurnSearch";
		AIState "tinycalebBurnSearch", "+3", kAiStateSearch, 3600, null, null, aiMoveForward, burnThinkSearch, "tinycalebBurnSearch";
		AIState "tinycalebBurnAttack", "+3", kAiStateChase, 120, BurnSeqCallback, null, null, null, "tinycalebBurnChase";
		AIState "tinycalebIdle", "+0", kAiStateIdle, 0, null, null, null, aiThinkTarget, "none";
		AIState "tinycalebChase", "+6", kAiStateChase, 0, null, null, aiMoveForward, calebThinkChase, "none";
		AIState "tinycalebDodge", "+6", kAiStateMove, 90, null, null, aiMoveDodge, null, "tinycalebChase";
		AIState "tinycalebGoto", "+6", kAiStateMove, 600, null, null, aiMoveForward, calebThinkGoto, "tinycalebIdle";
		AIState "tinycalebAttack", "+0", kAiStateChase, 120, SeqAttackCallback, null, null, null, "tinycalebChase";
		AIState "tinycalebSearch", "+6", kAiStateSearch, 120, null, null, aiMoveForward, calebThinkSearch, "tinycalebIdle";
		AIState "tinycalebRecoil", "+5", kAiStateRecoil, 0, null, null, null, null, "tinycalebDodge";
		AIState "tinycalebTeslaRecoil", "+4", kAiStateRecoil, 0, null, null, null, null, "tinycalebDodge";
		AIState "tinycalebSwimIdle", "+10", kAiStateIdle, 0, null, null, null, aiThinkTarget, "none";
		AIState "tinycalebSwimChase", "+8", kAiStateChase, 0, null, null, calebMoveSwimChase, calebThinkSwimChase, "none";
		AIState "tinycalebSwimDodge", "+8", kAiStateMove, 90, null, null, aiMoveDodge, null, "tinycalebSwimChase";
		AIState "tinycalebSwimGoto", "+8", kAiStateMove, 600, null, null, aiMoveForward, calebThinkSwimGoto, "tinycalebSwimIdle";
		AIState "tinycalebSwimSearch", "+8", kAiStateSearch, 120, null, null, aiMoveForward, calebThinkSearch, "tinycalebSwimIdle";
		AIState "tinycalebSwimAttack", "+10", kAiStateChase, 0, SeqAttackCallback, null, null, null, "tinycalebSwimChase";
		AIState "tinycalebSwimRecoil", "+5", kAiStateRecoil, 0, null, null, null, null, "tinycalebSwimDodge";
		AIState "tinycalebSwimUnused", "+8", kAiStateOther, 120, null, null, calebSwimUnused, calebThinkSwimChase, "tinycalebSwimChase";
		AIState "tinycalebSwimMoveIn", "+8", kAiStateOther, 0, null, null, calebSwimMoveIn, calebThinkSwimChase, "tinycalebSwimChase";
		AIState "tinycalebSwimTurn", "+8", kAiStateOther, 120, null, null, aiMoveTurn, null, "tinycalebSwimChase";
		AIState "cerberusIdle", "+0", kAiStateIdle, 0, null, null, null, cerberusThinkTarget, "none";
		AIState "cerberusSearch", "+7", kAiStateSearch, 1800, null, null, aiMoveForward, cerberusThinkSearch, "cerberusIdle";
		AIState "cerberusChase", "+7", kAiStateChase, 0, null, null, aiMoveForward, cerberusThinkChase, "none";
		AIState "cerberusRecoil", "+5", kAiStateRecoil, 0, null, null, null, null, "cerberusSearch";
		AIState "cerberusTeslaRecoil", "+4", kAiStateRecoil, 0, null, null, null, null, "cerberusSearch";
		AIState "cerberusGoto", "+7", kAiStateMove, 600, null, null, aiMoveForward, cerberusThinkGoto, "cerberusIdle";
		AIState "cerberusBite", "+6", kAiStateChase, 60, cerberusBiteSeqCallback, null, null, null, "cerberusChase";
		AIState "cerberusBurn", "+6", kAiStateChase, 60, cerberusBurnSeqCallback, null, null, null, "cerberusChase";
		AIState "cerberus3Burn", "+6", kAiStateChase, 60, cerberusBurnSeqCallback2, null, null, null, "cerberusChase";
		AIState "cerberus2Idle", "+0", kAiStateIdle, 0, null, null, null, cerberusThinkTarget, "none";
		AIState "cerberus2Search", "+7", kAiStateSearch, 1800, null, null, aiMoveForward, cerberusThinkSearch, "cerberus2Idle";
		AIState "cerberus2Chase", "+7", kAiStateChase, 0, null, null, aiMoveForward, cerberusThinkChase, "none";
		AIState "cerberus2Recoil", "+5", kAiStateRecoil, 0, null, null, null, null, "cerberus2Search";
		AIState "cerberus2Goto", "+7", kAiStateMove, 600, null, null, aiMoveForward, cerberusThinkGoto, "cerberus2Idle";
		AIState "cerberus2Bite", "+6", kAiStateChase, 60, cerberusBiteSeqCallback, null, null, null, "cerberus2Chase";
		AIState "cerberus2Burn", "+6", kAiStateChase, 60, cerberusBurnSeqCallback, null, null, null, "cerberus2Chase";
		AIState "cerberus4Burn", "+6", kAiStateChase, 60, cerberusBurnSeqCallback2, null, null, null, "cerberus2Chase";
		AIState "cerberusTurn1", "+7", kAiStateOther, 120, null, null, aiMoveTurn, null, "cerberusChase";
		AIState "cerberusTurn2", "+7", kAiStateOther, 120, null, null, aiMoveTurn, null, "cerberusChase";
		AIState "cultistIdle", "+0", kAiStateIdle, 0, null, null, null, aiThinkTarget, "none";
		AIState "cultistProneIdle", "+17", kAiStateIdle, 0, null, null, null, aiThinkTarget, "none";
		AIState "fanaticProneIdle", "+17", kAiStateIdle, 0, null, null, null, aiThinkTarget, "none";
		AIState "cultistProneIdle3", "+17", kAiStateIdle, 0, null, null, null, aiThinkTarget, "none";
		AIState "cultistChase", "+9", kAiStateChase, 0, null, null, aiMoveForward, cultThinkChase, "none";
		AIState "fanaticChase", "+0", kAiStateChase, 0, null, null, aiMoveTurn, cultThinkChase, "none";
		AIState "cultistDodge", "+9", kAiStateMove, 90, null, null, aiMoveDodge, null, "cultistChase";
		AIState "cultistGoto", "+9", kAiStateMove, 600, null, null, aiMoveForward, cultThinkGoto, "cultistIdle";
		AIState "cultistProneChase", "+14", kAiStateChase, 0, null, null, aiMoveForward, cultThinkChase, "none";
		AIState "cultistProneDodge", "+14", kAiStateMove, 90, null, null, aiMoveDodge, null, "cultistProneChase";
		AIState "cultistTThrow", "+7", kAiStateChase, 120, cultThrowSeqCallback, null, null, null, "cultistTFire";
		AIState "cultistSThrow", "+7", kAiStateChase, 120, cultThrowSeqCallback, null, null, null, "cultistSFire";
		AIState "cultistTsThrow", "+7", kAiStateChase, 120, cultThrowSeqCallback, null, null, null, "cultistTsFire";
		AIState "cultistDThrow", "+7", kAiStateChase, 120, cultThrowSeqCallback, null, null, null, "cultistChase";
		AIState "cultistDThrow2", "+7", kAiStateChase, 120, cultThrowSeqCallback2, null, null, null, "cultistChase";
		AIState "cultistDThrow3C", "+7", kAiStateChase, 120, cultThrowSeqCallback3, null, null, null, "cultistIdle";
		AIState "cultistDThrow3B", "+7", kAiStateChase, 120, cultThrowSeqCallback3, null, null, cultThinkSearch, "cultistDThrow3C";
		AIState "cultistDThrow3A", "+7", kAiStateChase, 120, cultThrowSeqCallback3, null, null, cultThinkSearch, "cultistDThrow3B";
		AIState "cultistDThrow4", "+7", kAiStateChase, 120, cultThrowSeqCallback3, null, null, cultThinkSearch, "cultistDThrow4";
		AIState "cultistSearch", "+9", kAiStateSearch, 1800, null, null, aiMoveForward, cultThinkSearch, "cultistIdle";
		AIState "cultistSFire", "+6", kAiStateChase, 60, ShotSeqCallback, null, null, null, "cultistChase";
		AIState "cultistTFire", "+6", kAiStateChase, 0, TommySeqCallback, null, aiMoveTurn, cultThinkChase, "cultistTFire";
		AIState "cultistTsFire", "+6", kAiStateChase, 0, TeslaSeqCallback, null, aiMoveTurn, cultThinkChase, "cultistChase";
		AIState "cultistSProneFire", "+8", kAiStateChase, 60, ShotSeqCallback, null, null, null, "cultistProneChase";
		AIState "cultistTProneFire", "+8", kAiStateChase, 0, TommySeqCallback, null, aiMoveTurn, cultThinkChase, "cultistTProneFire";
		AIState "cultistTsProneFire", "+8", kAiStateChase, 0, TeslaSeqCallback, null, aiMoveTurn, null, "cultistTsProneFire";
		AIState "cultistTsProneFireFixed", "+8", kAiStateChase, 0, TeslaSeqCallback, null, aiMoveTurn, cultThinkChase, "cultistTsProneFireFixed";
		AIState "cultistRecoil", "+5", kAiStateRecoil, 0, null, null, null, null, "cultistDodge";
		AIState "cultistProneRecoil", "+5", kAiStateRecoil, 0, null, null, null, null, "cultistProneDodge";
		AIState "cultistTeslaRecoil", "+4", kAiStateRecoil, 0, null, null, null, null, "cultistDodge";
		AIState "cultistSwimIdle", "+13", kAiStateIdle, 0, null, null, null, aiThinkTarget, "none";
		AIState "cultistSwimChase", "+13", kAiStateChase, 0, null, null, aiMoveForward, cultThinkChase, "none";
		AIState "cultistSwimDodge", "+13", kAiStateMove, 90, null, null, aiMoveDodge, null, "cultistSwimChase";
		AIState "cultistSwimGoto", "+13", kAiStateMove, 600, null, null, aiMoveForward, cultThinkGoto, "cultistSwimIdle";
		AIState "cultistSwimSearch", "+13", kAiStateSearch, 1800, null, null, aiMoveForward, cultThinkSearch, "cultistSwimIdle";
		AIState "cultistSSwimFire", "+8", kAiStateChase, 60, ShotSeqCallback, null, null, null, "cultistSwimChase";
		AIState "cultistTSwimFire", "+8", kAiStateChase, 0, TommySeqCallback, null, aiMoveTurn, cultThinkChase, "cultistTSwimFire";
		AIState "cultistTsSwimFire", "+8", kAiStateChase, 0, TeslaSeqCallback, null, aiMoveTurn, cultThinkChase, "cultistTsSwimFire";
		AIState "cultistSwimRecoil", "+5", kAiStateRecoil, 0, null, null, null, null, "cultistSwimDodge";
		AIState "gargoyleFIdle", "+0", kAiStateIdle, 0, null, null, null, gargThinkTarget, "none";
		AIState "gargoyleStatueIdle", "+0", kAiStateIdle, 0, null, null, null, null, "none";
		AIState "gargoyleFChase", "+0", kAiStateChase, 0, null, null, gargMoveForward, gargThinkChase, "gargoyleFIdle";
		AIState "gargoyleFGoto", "+0", kAiStateMove, 600, null, null, gargMoveForward, gargThinkGoto, "gargoyleFIdle";
		AIState "gargoyleFSlash", "+6", kAiStateChase, 120, SlashFSeqCallback, null, null, null, "gargoyleFChase";
		AIState "gargoyleFThrow", "+6", kAiStateChase, 120, ThrowFSeqCallback, null, null, null, "gargoyleFChase";
		AIState "gargoyleSThrow", "+6", kAiStateChase, 120, ThrowSSeqCallback, null, gargMoveForward, null, "gargoyleFChase";
		AIState "gargoyleSBlast", "+7", kAiStateChase, 60, BlastSSeqCallback, null, gargMoveSlow, null, "gargoyleFChase";
		AIState "gargoyleFRecoil", "+5", kAiStateRecoil, 0, null, null, null, null, "gargoyleFChase";
		AIState "gargoyleFSearch", "+0", kAiStateSearch, 120, null, null, gargMoveForward, gargThinkSearch, "gargoyleFIdle";
		AIState "gargoyleFMorph2", "+-1", kAiStateOther, 0, null, entryFStatue, null, null, "gargoyleFIdle";
		AIState "gargoyleFMorph", "+6", kAiStateOther, 0, null, null, null, null, "gargoyleFMorph2";
		AIState "gargoyleSMorph2", "+-1", kAiStateOther, 0, null, entrySStatue, null, null, "gargoyleFIdle";
		AIState "gargoyleSMorph", "+6", kAiStateOther, 0, null, null, null, null, "gargoyleSMorph2";
		AIState "gargoyleSwoop", "+0", kAiStateOther, 120, null, null, gargMoveSwoop, gargThinkChase, "gargoyleFChase";
		AIState "gargoyleFly", "+0", kAiStateMove, 120, null, null, gargMoveFly, gargThinkChase, "gargoyleFChase";
		AIState "gargoyleTurn", "+0", kAiStateMove, 120, null, null, aiMoveTurn, null, "gargoyleFChase";
		AIState "gargoyleDodgeUp", "+0", kAiStateMove, 60, null, null, gargMoveDodgeUp, null, "gargoyleFChase";
		AIState "gargoyleFDodgeUpRight", "+0", kAiStateMove, 90, null, null, gargMoveDodgeUp, null, "gargoyleFChase";
		AIState "gargoyleFDodgeUpLeft", "+0", kAiStateMove, 90, null, null, gargMoveDodgeUp, null, "gargoyleFChase";
		AIState "gargoyleDodgeDown", "+0", kAiStateMove, 120, null, null, gargMoveDodgeDown, null, "gargoyleFChase";
		AIState "gargoyleFDodgeDownRight", "+0", kAiStateMove, 90, null, null, gargMoveDodgeDown, null, "gargoyleFChase";
		AIState "gargoyleFDodgeDownLeft", "+0", kAiStateMove, 90, null, null, gargMoveDodgeDown, null, "gargoyleFChase";
		AIState "statueFBreakSEQ", "+5", kAiStateOther, 0, null, entryFStatue, null, playStatueBreakSnd, "gargoyleFMorph2";
		AIState "statueSBreakSEQ", "+5", kAiStateOther, 0, null, entrySStatue, null, playStatueBreakSnd, "gargoyleSMorph2";
		AIState "ghostIdle", "+0", kAiStateIdle, 0, null, null, null, ghostThinkTarget, "none";
		AIState "ghostChase", "+0", kAiStateChase, 0, null, null, ghostMoveForward, ghostThinkChase, "ghostIdle";
		AIState "ghostGoto", "+0", kAiStateMove, 600, null, null, ghostMoveForward, ghostThinkGoto, "ghostIdle";
		AIState "ghostSlash", "+6", kAiStateChase, 120, ghostSlashSeqCallback, null, null, null, "ghostChase";
		AIState "ghostThrow", "+6", kAiStateChase, 120, ghostThrowSeqCallback, null, null, null, "ghostChase";
		AIState "ghostBlast", "+6", kAiStateChase, 120, ghostBlastSeqCallback, null, ghostMoveSlow, null, "ghostChase";
		AIState "ghostRecoil", "+5", kAiStateRecoil, 0, null, null, null, null, "ghostChase";
		AIState "ghostTeslaRecoil", "+4", kAiStateRecoil, 0, null, null, null, null, "ghostChase";
		AIState "ghostSearch", "+0", kAiStateSearch, 120, null, null, ghostMoveForward, ghostThinkSearch, "ghostIdle";
		AIState "ghostSwoop", "+0", kAiStateOther, 120, null, null, ghostMoveSwoop, ghostThinkChase, "ghostChase";
		AIState "ghostFly", "+0", kAiStateMove, 0, null, null, ghostMoveFly, ghostThinkChase, "ghostChase";
		AIState "ghostTurn", "+0", kAiStateMove, 120, null, null, aiMoveTurn, null, "ghostChase";
		AIState "ghostDodgeUp", "+0", kAiStateMove, 60, null, null, ghostMoveDodgeUp, null, "ghostChase";
		AIState "ghostDodgeUpRight", "+0", kAiStateMove, 90, null, null, ghostMoveDodgeUp, null, "ghostChase";
		AIState "ghostDodgeUpLeft", "+0", kAiStateMove, 90, null, null, ghostMoveDodgeUp, null, "ghostChase";
		AIState "ghostDodgeDown", "+0", kAiStateMove, 120, null, null, ghostMoveDodgeDown, null, "ghostChase";
		AIState "ghostDodgeDownRight", "+0", kAiStateMove, 90, null, null, ghostMoveDodgeDown, null, "ghostChase";
		AIState "ghostDodgeDownLeft", "+0", kAiStateMove, 90, null, null, ghostMoveDodgeDown, null, "ghostChase";
		AIState "gillBeastIdle", "+0", kAiStateIdle, 0, null, null, null, aiThinkTarget, "none";
		AIState "gillBeastChase", "+9", kAiStateChase, 0, null, null, aiMoveForward, gillThinkChase, "none";
		AIState "gillBeastDodge", "+9", kAiStateMove, 90, null, null, aiMoveDodge, null, "gillBeastChase";
		AIState "gillBeastGoto", "+9", kAiStateMove, 600, null, null, aiMoveForward, gillThinkGoto, "gillBeastIdle";
		AIState "gillBeastBite", "+6", kAiStateChase, 120, GillBiteSeqCallback, null, null, null, "gillBeastChase";
		AIState "gillBeastSearch", "+9", kAiStateMove, 120, null, null, aiMoveForward, gillThinkSearch, "gillBeastIdle";
		AIState "gillBeastRecoil", "+5", kAiStateRecoil, 0, null, null, null, null, "gillBeastDodge";
		AIState "gillBeastSwimIdle", "+10", kAiStateIdle, 0, null, null, null, aiThinkTarget, "none";
		AIState "gillBeastSwimChase", "+10", kAiStateChase, 0, null, null, gillMoveSwimChase, gillThinkSwimChase, "none";
		AIState "gillBeastSwimDodge", "+10", kAiStateMove, 90, null, null, aiMoveDodge, null, "gillBeastSwimChase";
		AIState "gillBeastSwimGoto", "+10", kAiStateMove, 600, null, null, aiMoveForward, gillThinkSwimGoto, "gillBeastSwimIdle";
		AIState "gillBeastSwimSearch", "+10", kAiStateSearch, 120, null, null, aiMoveForward, gillThinkSearch, "gillBeastSwimIdle";
		AIState "gillBeastSwimBite", "+7", kAiStateChase, 0, GillBiteSeqCallback, null, null, gillThinkSwimChase, "gillBeastSwimChase";
		AIState "gillBeastSwimRecoil", "+5", kAiStateRecoil, 0, null, null, null, null, "gillBeastSwimDodge";
		AIState "gillBeastSwimUnused", "+10", kAiStateOther, 120, null, null, gillMoveSwimUnused, gillThinkSwimChase, "gillBeastSwimChase";
		AIState "gillBeastSwimMoveIn", "+10", kAiStateOther, 0, null, null, gillSwimMoveIn, gillThinkSwimChase, "gillBeastSwimChase";
		AIState "gillBeastSwimTurn", "+10", kAiStateOther, 120, null, null, null, aiMoveTurn, "gillBeastSwimChase";
		AIState "handIdle", "+0", kAiStateIdle, 0, null, null, null, aiThinkTarget, "none";
		AIState "hand13A3B4", "+0", kAiStateOther, 0, null, null, null, null, "none";
		AIState "handSearch", "+6", kAiStateMove, 600, null, null, aiMoveForward, handThinkSearch, "handIdle";
		AIState "handChase", "+6", kAiStateChase, 0, null, null, aiMoveForward, handThinkChase, "none";
		AIState "handRecoil", "+5", kAiStateRecoil, 0, null, null, null, null, "handSearch";
		AIState "handGoto", "+6", kAiStateMove, 1800, null, null, aiMoveForward, handThinkGoto, "handIdle";
		AIState "handJump", "+7", kAiStateChase, 120, HandJumpSeqCallback, null, null, null, "handChase";
		AIState "houndIdle", "+0", kAiStateIdle, 0, null, null, null, aiThinkTarget, "none";
		AIState "houndSearch", "+8", kAiStateMove, 1800, null, null, aiMoveForward, houndThinkSearch, "houndIdle";
		AIState "houndChase", "+8", kAiStateChase, 0, null, null, aiMoveForward, houndThinkChase, "none";
		AIState "houndRecoil", "+5", kAiStateRecoil, 0, null, null, null, null, "houndSearch";
		AIState "houndTeslaRecoil", "+4", kAiStateRecoil, 0, null, null, null, null, "houndSearch";
		AIState "houndGoto", "+8", kAiStateMove, 600, null, null, aiMoveForward, houndThinkGoto, "houndIdle";
		AIState "houndBite", "+6", kAiStateChase, 60, houndBiteSeqCallback, null, null, null, "houndChase";
		AIState "houndBurn", "+7", kAiStateChase, 60, houndBurnSeqCallback, null, null, null, "houndChase";
		AIState "innocentIdle", "+0", kAiStateIdle, 0, null, null, null, aiThinkTarget, "none";
		AIState "innocentSearch", "+6", kAiStateSearch, 1800, null, null, aiMoveForward, innocThinkSearch, "innocentIdle";
		AIState "innocentChase", "+6", kAiStateChase, 0, null, null, aiMoveForward, innocThinkChase, "none";
		AIState "innocentRecoil", "+5", kAiStateRecoil, 0, null, null, null, null, "innocentChase";
		AIState "innocentTeslaRecoil", "+4", kAiStateRecoil, 0, null, null, null, null, "innocentChase";
		AIState "innocentGoto", "+6", kAiStateMove, 600, null, null, aiMoveForward, innocThinkGoto, "innocentIdle";
		AIState "podIdle", "+0", kAiStateIdle, 0, null, null, null, aiThinkTarget, "none";
		AIState "podMove", "+7", kAiStateMove, 3600, null, null, aiMoveTurn, aiPodMove, "podSearch";
		AIState "podSearch", "+0", kAiStateSearch, 3600, null, null, aiMoveTurn, aiPodSearch, "podSearch";
		AIState "podStartChase", "+8", kAiStateChase, 600, podAttack, null, null, null, "podChase";
		AIState "podRecoil", "+5", kAiStateRecoil, 0, null, null, null, null, "podChase";
		AIState "podChase", "+6", kAiStateChase, 0, null, null, aiMoveTurn, aiPodChase, "none";
		AIState "tentacleIdle", "+0", kAiStateIdle, 0, null, null, null, aiThinkTarget, "none";
		AIState "tentaclePlaySound1", "+7", kAiStateOther, 0, podPlaySound1, null, null, null, "tentaclePlaySound1A";
		AIState "tentaclePlaySound1A", "+-1", kAiStateOther, 0, null, null, null, null, "tentacleChase";
		AIState "tentaclePlaySound2", "+8", kAiStateOther, 0, podPlaySound2, null, null, null, "tentaclePlaySound2A";
		AIState "tentaclePlaySound2A", "+-1", kAiStateOther, 0, null, null, null, null, "tentacleIdle";
		AIState "tentacleMove", "+8", kAiStateOther, 3600, null, null, aiMoveTurn, aiPodMove, "tentacleSearch";
		AIState "tentacleSearch", "+0", kAiStateOther, 3600, null, null, aiMoveTurn, aiPodSearch, "none";
		AIState "tentacleStartChase", "+6", kAiStateOther, 120, podExplode, null, null, null, "tentacleChase";
		AIState "tentacleRecoil", "+5", kAiStateRecoil, 0, null, null, null, null, "tentacleChase";
		AIState "tentacleChase", "+6", kAiStateChase, 0, null, null, aiMoveTurn, aiPodChase, "none";
		AIState "ratIdle", "+0", kAiStateIdle, 0, null, null, null, aiThinkTarget, "none";
		AIState "ratSearch", "+7", kAiStateSearch, 1800, null, null, aiMoveForward, ratThinkSearch, "ratIdle";
		AIState "ratChase", "+7", kAiStateChase, 0, null, null, aiMoveForward, ratThinkChase, "none";
		AIState "ratDodge", "+7", kAiStateMove, 0, null, null, null, null, "ratChase";
		AIState "ratRecoil", "+7", kAiStateRecoil, 0, null, null, null, null, "ratDodge";
		AIState "ratGoto", "+7", kAiStateMove, 600, null, null, aiMoveForward, ratThinkGoto, "ratIdle";
		AIState "ratBite", "+6", kAiStateChase, 120, ratBiteSeqCallback, null, null, null, "ratChase";
		AIState "spidIdle", "+0", kAiStateIdle, 0, null, null, null, aiThinkTarget, "none";
		AIState "spidChase", "+7", kAiStateChase, 0, null, null, aiMoveForward, spidThinkChase, "none";
		AIState "spidDodge", "+7", kAiStateMove, 90, null, null, aiMoveDodge, null, "spidChase";
		AIState "spidGoto", "+7", kAiStateMove, 600, null, null, aiMoveForward, spidThinkGoto, "spidIdle";
		AIState "spidSearch", "+7", kAiStateSearch, 1800, null, null, aiMoveForward, spidThinkSearch, "spidIdle";
		AIState "spidBite", "+6", kAiStateChase, 60, SpidBiteSeqCallback, null, null, null, "spidChase";
		AIState "spidJump", "+8", kAiStateChase, 60, SpidJumpSeqCallback, null, aiMoveForward, null, "spidChase";
		AIState "spidBirth", "+0", kAiStateOther, 60, SpidBirthSeqCallback, null, null, null, "spidIdle";
		AIState "tchernobogIdle", "+0", kAiStateIdle, 0, null, null, null, tchernobogThinkTarget, "none";
		AIState "tchernobogSearch", "+8", kAiStateSearch, 1800, null, null, aiMoveForward, tchernobogThinkSearch, "tchernobogIdle";
		AIState "tchernobogChase", "+8", kAiStateChase, 0, null, null, aiMoveForward, tchernobogThinkChase, "none";
		AIState "tchernobogRecoil", "+5", kAiStateRecoil, 0, null, null, null, null, "tchernobogSearch";
		AIState "tchernobogGoto", "+8", kAiStateMove, 600, null, null, aiMoveForward, tchernobogThinkGoto, "tchernobogIdle";
		AIState "tchernobogBurn1", "+6", kAiStateMove, 60, tchernobogBurnSeqCallback, null, null, null, "tchernobogChase";
		AIState "tchernobogBurn2", "+6", kAiStateChase, 60, tchernobogBurnSeqCallback2, null, null, null, "tchernobogChase";
		AIState "tchernobogFireAtk", "+7", kAiStateChase, 60, tchernobogFire, null, null, null, "tchernobogChase";
		AIState "tchernobogTurn", "+8", kAiStateChase, 60, null, null, aiMoveTurn, null, "tchernobogChase";
		AIState "zombieAIdle", "+0", kAiStateIdle, 0, null, entryAIdle, null, aiThinkTarget, "none";
		AIState "zombieAChase", "+8", kAiStateChase, 0, null, null, aiMoveForward, zombaThinkChase, "none";
		AIState "zombieAPonder", "+0", kAiStateOther, 0, null, null, aiMoveTurn, zombaThinkPonder, "none";
		AIState "zombieAGoto", "+8", kAiStateMove, 1800, null, null, aiMoveForward, zombaThinkGoto, "zombieAIdle";
		AIState "zombieAHack", "+6", kAiStateChase, 80, HackSeqCallback, null, null, null, "zombieAPonder";
		AIState "zombieASearch", "+8", kAiStateSearch, 1800, null, null, aiMoveForward, zombaThinkSearch, "zombieAIdle";
		AIState "zombieARecoil", "+5", kAiStateRecoil, 0, null, null, null, null, "zombieAPonder";
		AIState "zombieATeslaRecoil", "+4", kAiStateRecoil, 0, null, null, null, null, "zombieAPonder";
		AIState "zombieARecoil2", "+1", kAiStateRecoil, 360, null, null, null, null, "zombieAStand";
		AIState "zombieAStand", "+11", kAiStateMove, 0, StandSeqCallback, null, null, null, "zombieAPonder";
		AIState "zombieEIdle", "+12", kAiStateIdle, 0, null, null, null, aiThinkTarget, "none";
		AIState "zombieEUp2", "+0", kAiStateMove, 1, null, entryEZombie, null, null, "zombieASearch";
		AIState "zombieEUp", "+9", kAiStateMove, 180, null, entryEStand, null, null, "zombieEUp2";
		AIState "zombie2Idle", "+0", kAiStateIdle, 0, null, entryAIdle, null, myThinkTarget, "none";
		AIState "zombie2Search", "+8", kAiStateSearch, 1800, null, null, null, myThinkSearch, "zombie2Idle";
		AIState "zombieSIdle", "+10", kAiStateIdle, 0, null, null, null, aiThinkTarget, "none";
		AIState "zombieEStand", "+11", kAiStateOther, 0, StandSeqCallback, entryEZombie, null, null, "zombieAPonder";
		AIState "zombieFIdle", "+0", kAiStateIdle, 0, null, null, null, aiThinkTarget, "none";
		AIState "zombieFChase", "+8", kAiStateChase, 0, null, null, aiMoveForward, zombfThinkChase, "none";
		AIState "zombieFGoto", "+8", kAiStateMove, 600, null, null, aiMoveForward, zombfThinkGoto, "zombieFIdle";
		AIState "zombieFDodge", "+8", kAiStateMove, 0, null, null, aiMoveDodge, zombfThinkChase, "zombieFChase";
		AIState "zombieFHack", "+6", kAiStateChase, 120, zombfHackSeqCallback, null, null, null, "zombieFChase";
		AIState "zombieFPuke", "+9", kAiStateChase, 120, PukeSeqCallback, null, null, null, "zombieFChase";
		AIState "zombieFThrow", "+6", kAiStateChase, 120, ThrowSeqCallback, null, null, null, "zombieFChase";
		AIState "zombieFSearch", "+8", kAiStateSearch, 1800, null, null, aiMoveForward, zombfThinkSearch, "zombieFIdle";
		AIState "zombieFRecoil", "+5", kAiStateRecoil, 0, null, null, null, null, "zombieFChase";
		AIState "zombieFTeslaRecoil", "+4", kAiStateRecoil, 0, null, null, null, null, "zombieFChase";
		AIState "genPatrolState_0", "+0", kAiStatePatrolWaitL, 0, null, null, null, aiPatrolThink, "none";
		AIState "genPatrolState_1", "+7", kAiStatePatrolWaitL, 0, null, null, null, aiPatrolThink, "none";
		AIState "genPatrolState_2", "+9", kAiStatePatrolMoveL, 0, null, null, aiPatrolMove, aiPatrolThink, "none";
		AIState "genPatrolState_3", "+8", kAiStatePatrolMoveL, 0, null, null, aiPatrolMove, aiPatrolThink, "none";
		AIState "genPatrolState_4", "+0", kAiStatePatrolMoveL, 0, null, null, aiPatrolMove, aiPatrolThink, "none";
		AIState "genPatrolState_5", "+6", kAiStatePatrolMoveL, 0, null, null, aiPatrolMove, aiPatrolThink, "none";
		AIState "genPatrolState_6", "+7", kAiStatePatrolMoveL, 0, null, null, aiPatrolMove, aiPatrolThink, "none";
		AIState "genPatrolState_7", "+3", kAiStatePatrolMoveL, 0, null, null, aiPatrolMove, aiPatrolThink, "none";
		AIState "genPatrolState_8", "+9", kAiStatePatrolTurnL, 0, null, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, "none";
		AIState "genPatrolState_9", "+8", kAiStatePatrolTurnL, 0, null, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, "none";
		AIState "genPatrolState_10", "+0", kAiStatePatrolTurnL, 0, null, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, "none";
		AIState "genPatrolState_11", "+6", kAiStatePatrolTurnL, 0, null, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, "none";
		AIState "genPatrolState_12", "+7", kAiStatePatrolTurnL, 0, null, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, "none";
		AIState "genPatrolState_13", "+3", kAiStatePatrolTurnL, 0, null, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, "none";
		AIState "genPatrolState_14", "+0", kAiStatePatrolWaitW, 0, null, null, null, aiPatrolThink, "none";
		AIState "genPatrolState_15", "+10", kAiStatePatrolWaitW, 0, null, null, null, aiPatrolThink, "none";
		AIState "genPatrolState_16", "+13", kAiStatePatrolWaitW, 0, null, null, null, aiPatrolThink, "none";
		AIState "genPatrolState_17", "+17", kAiStatePatrolWaitW, 0, null, null, null, aiPatrolThink, "none";
		AIState "genPatrolState_18", "+8", kAiStatePatrolWaitW, 0, null, null, null, aiPatrolThink, "none";
		AIState "genPatrolState_19", "+9", kAiStatePatrolWaitW, 0, null, null, null, aiPatrolThink, "none";
		AIState "genPatrolState_20", "+2", kAiStatePatrolWaitW, 0, null, null, null, aiPatrolThink, "none";
		AIState "genPatrolState_21", "+0", kAiStatePatrolMoveW, 0, null, null, aiPatrolMove, aiPatrolThink, "none";
		AIState "genPatrolState_22", "+10", kAiStatePatrolMoveW, 0, null, null, aiPatrolMove, aiPatrolThink, "none";
		AIState "genPatrolState_23", "+13", kAiStatePatrolMoveW, 0, null, null, aiPatrolMove, aiPatrolThink, "none";
		AIState "genPatrolState_24", "+8", kAiStatePatrolMoveW, 0, null, null, aiPatrolMove, aiPatrolThink, "none";
		AIState "genPatrolState_25", "+9", kAiStatePatrolMoveW, 0, null, null, aiPatrolMove, aiPatrolThink, "none";
		AIState "genPatrolState_26", "+7", kAiStatePatrolMoveW, 0, null, null, aiPatrolMove, aiPatrolThink, "none";
		AIState "genPatrolState_27", "+6", kAiStatePatrolMoveW, 0, null, null, aiPatrolMove, aiPatrolThink, "none";
		AIState "genPatrolState_28", "+5", kAiStatePatrolMoveW, 0, null, null, aiPatrolMove, aiPatrolThink, "none";
		AIState "genPatrolState_29", "+0", kAiStatePatrolTurnW, 0, null, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, "none";
		AIState "genPatrolState_30", "+10", kAiStatePatrolTurnW, 0, null, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, "none";
		AIState "genPatrolState_31", "+13", kAiStatePatrolTurnW, 0, null, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, "none";
		AIState "genPatrolState_32", "+8", kAiStatePatrolTurnW, 0, null, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, "none";
		AIState "genPatrolState_33", "+9", kAiStatePatrolTurnW, 0, null, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, "none";
		AIState "genPatrolState_34", "+7", kAiStatePatrolTurnW, 0, null, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, "none";
		AIState "genPatrolState_35", "+6", kAiStatePatrolTurnW, 0, null, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, "none";
		AIState "genPatrolState_36", "+5", kAiStatePatrolTurnW, 0, null, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, "none";
		AIState "genPatrolState_37", "+17", kAiStatePatrolWaitC, 0, null, null, null, aiPatrolThink, "none";
		AIState "genPatrolState_38", "+11", kAiStatePatrolWaitC, 0, null, null, null, aiPatrolThink, "none";
		AIState "genPatrolState_39", "+10", kAiStatePatrolWaitC, 0, null, null, null, aiPatrolThink, "none";
		AIState "genPatrolState_40", "+14", kAiStatePatrolWaitC, 0, null, null, null, aiPatrolThink, "none";
		AIState "genPatrolState_41", "+1", kAiStatePatrolWaitC, 0, null, null, null, aiPatrolThink, "none";
		AIState "genPatrolState_42", "+14", kAiStatePatrolMoveC, 0, null, null, aiPatrolMove, aiPatrolThink, "none";
		AIState "genPatrolState_43", "+10", kAiStatePatrolMoveC, 0, null, null, aiPatrolMove, aiPatrolThink, "none";
		AIState "genPatrolState_44", "+9", kAiStatePatrolMoveC, 0, null, null, aiPatrolMove, aiPatrolThink, "none";
		AIState "genPatrolState_45", "+4", kAiStatePatrolMoveC, 0, null, null, aiPatrolMove, aiPatrolThink, "none";
		AIState "genPatrolState_46", "+14", kAiStatePatrolTurnC, 0, null, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, "none";
		AIState "genPatrolState_47", "+10", kAiStatePatrolTurnC, 0, null, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, "none";
		AIState "genPatrolState_48", "+9", kAiStatePatrolTurnC, 0, null, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, "none";
		AIState "genPatrolState_49", "+4", kAiStatePatrolTurnC, 0, null, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, "none";
		AISTATE "cdudeDeath", "+-1", kAiStateOther, 0, null, enterDeath, null, null;

	}
}
