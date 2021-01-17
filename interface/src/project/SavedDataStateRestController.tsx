import React, { Component } from 'react';
import { ValidatorForm } from 'react-material-ui-form-validator';

import { Typography, Box,TextField, FormControlLabel, Switch} from '@material-ui/core';
import SaveIcon from '@material-ui/icons/Save';
import RefreshIcon from '@material-ui/icons/Refresh';

import { ENDPOINT_ROOT } from '../api';
import { restController, RestControllerProps, RestFormLoader, RestFormProps, FormActions, FormButton, SectionContent} from '../components';

import { SavedDataState } from './types';
//import { TextFieldsOutlined } from '@material-ui/icons';

export const LIGHT_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "SavedDataState";

type SavedDataStateRestControllerProps = RestControllerProps<SavedDataState>;

class SavedDataStateRestController extends Component<SavedDataStateRestControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title='Données podomatic' titleGutter>
        <RestFormLoader
          {...this.props}
          render={props => (
            <SavedDataStateRestControllerForm {...props} />
          )}
        />
      </SectionContent>
    )
  }

}

export default restController(LIGHT_SETTINGS_ENDPOINT, SavedDataStateRestController);

type SavedDataStateRestControllerFormProps = RestFormProps<SavedDataState>;

function refreshPage() {
  window.location.reload(false);
}

function SavedDataStateRestControllerForm(props: SavedDataStateRestControllerFormProps) {
  const { data, saveData, handleValueChange } = props;
  return (
    <ValidatorForm onSubmit={saveData}>
      <Box bgcolor="primary.main" color="primary.contrastText" p={2} mt={2} mb={2}>
        <Typography variant="body1">
          Historique
        </Typography>
      </Box>
      <FormControlLabel
        control={<Switch checked={data.Reset_counters} onChange={handleValueChange('Reset_counters')} name="checkedA" />}
        label="Remise à zero compteurs"
      />
      <Typography variant="subtitle2" gutterBottom>
            Temps total de spray en h : {data.temps_total_spray}
      </Typography>
      <Typography variant="subtitle2" gutterBottom>
            Nombre total de passages : {data.nb_total_passage}
      </Typography>
      <FormActions>
        <FormButton startIcon={<RefreshIcon />} variant="contained" color="primary" onClick={refreshPage}>
          Rafraichir
        </FormButton>
      </FormActions>
      <Box bgcolor="primary.main" color="primary.contrastText" p={2} mt={2} mb={2}>
        <Typography variant="body1">
        Reglage des parametres :
        </Typography>
      </Box>
      <TextField id="standard-basic" label="Temps de spray(ms)" onChange={handleValueChange('MS_SPRAY')} value={data.MS_SPRAY} color="secondary" />
      <TextField id="standard-basic" label="Retard demarrage(ms)" onChange={handleValueChange('MS_RETARD_DEMARRAGE')} value={data.MS_RETARD_DEMARRAGE} color="secondary" />
      <TextField id="standard-basic" label="Temps arret (ms)" onChange={handleValueChange('MS_Arret')} value={data.MS_Arret} color="secondary" />
      <TextField id="standard-basic" label="Level min cuve(mm)" onChange={handleValueChange('D_Min_level_cuve')} value={data.D_Min_level_cuve} color="secondary" />
      <FormActions>
        <FormButton startIcon={<SaveIcon />} variant="contained" color="primary" type="submit">
          Save
        </FormButton>
        </FormActions>

    </ValidatorForm>
  );
}
